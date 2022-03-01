import pdfquery

from pdfscraper.models.sensor import Sensor
from pdfscraper.models.manufacturer import Manufacturer
from pdfscraper.parsers import parser_utils
from pdfscraper.parsers.parser_utils import Justification

U_COLON = '\uff1a'
U_LEFT_BRACKET = '\uff08'
U_RIGHT_BRACKET = '\uff09'
U_DEGREES = '\u2103'
U_TILDE = '\uff5e'
U_LESS_THAN = '\uff1c'
U_LESS_THAN_EQUAL = '\u2264'
U_PERCENT = '\ufe6a'
U_OHM = '\u03a9'
U_MICRO_SMALL = '\u00b5'
U_MICRO = '\u03bc'
U_PLUS_MINUS = '\u00b1'


class WinsenParserFormat1:
    def __init__(self, pdf):
        """Initialises the parser of Winsen datasheets with the table like
        format.

        Args:
            pdf (PDFQuery): A query wrapper around a supplied PDF for data to
                be extracted from.
        """
        self.pdf = pdf

    def parse(self):
        """Extracts each sensor specification from a Winsen datasheet.

        Returns:
            dict: A dictionary filled in the found values. Any values that
            could not be found will be defaulted to the None type.
        """
        sensor = Sensor()

        if 'Statement' not in parser_utils.search_text(self.pdf, 'Statement', page_num=None):
            page_num = 1
        else:
            page_num = 2
        text_rows = parser_utils.extract_tabular_data(self.pdf, x_tolerance=30, y_tolerance=6, page_num=page_num)
        # Data continues on the next page
        if parser_utils.find_row_with_substring(text_rows, 'temperature') is None:
            text_rows.extend(parser_utils.extract_tabular_data(self.pdf, x_tolerance=30, y_tolerance=6, page_num=page_num + 1))
        text_rows[:] = [row for row in text_rows if len(row) > 1]

        sensor.manufacturer = Manufacturer.WINSEN
        sensor.technology = 'EC'

        try:
            _part_number = parser_utils.search_text(self.pdf, 'Model')
            if _part_number != '':
                delimiter = ': ' if ': ' in _part_number else U_COLON
                sensor.part_number = _part_number.split(delimiter)[-1].replace(U_RIGHT_BRACKET, '')
            else:
                _part_number = parser_utils.search_text(self.pdf, 'Gas Sensor')
                if _part_number != '':
                    sensor.part_number = _part_number.split('(')[-1].replace(')', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.alias = sensor.part_number
        sensor.active = True
        sensor.datasheet_rev = None

        try:
            _pin_count = sensor.part_number.split('-')[0][-1]
            if _pin_count in ['2', '3']:
                sensor.pin_count = _pin_count
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        # Weight
        try:
            pass
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Storage temperature')
            if text_row is None:
                text_row = parser_utils.find_row_with_substring(text_rows, 'Storagetemperature')
            _temp = text_row[1].replace(U_DEGREES, '').split(U_TILDE)
            if _temp[0] != '' and _temp[1] != '':
                sensor.min_temp = _temp[0]
                sensor.max_temp = _temp[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _pressure = parser_utils.search_next_column(self.pdf, 'Pressure', (116, 240), search_n_rows=2.75, justification=Justification.RIGHT, page_num=page_num)
            if _pressure == '':
                _pressure = parser_utils.search_next_column(self.pdf, 'Pressure', (116, 240), search_n_rows=2.75, justification=Justification.RIGHT, page_num=page_num + 1)

            if '-' not in _pressure:
                _pressure = _pressure.split(U_PLUS_MINUS)[1].split()[0].replace('%', '').replace(U_PERCENT, '')
                sensor.min_pressure = 'Atmospheric - ' + _pressure + '%'
                sensor.max_pressure = 'Atmospheric + ' + _pressure + '%'
            else:
                sensor.min_pressure = _pressure.split()[0].split('-')[0]
                sensor.max_pressure = _pressure.split()[0].split('-')[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Humidity')
            delimiter = '%' if '%' in text_row[1] else U_PERCENT
            _humidity = text_row[1].replace(U_TILDE, '').split(delimiter)
            if _humidity[0] != '' and _humidity[1] != '':
                sensor.min_humidity = _humidity[0]
                sensor.max_humidity = _humidity[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Load')
            if text_row[1] != '':
                _load = text_row[1].split(U_OHM)[0].strip()
                if 'k' in _load:
                    _load = str(int(_load.replace('k', '')) * 1000)
                sensor.min_load = _load
                sensor.max_load = _load
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Bias')
            if text_row is not None and text_row[1] != '':
                _bias = text_row[1].replace('mV', '')
                if _bias != '0':
                    sensor.bias = _bias
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Stability')
            if text_row[1] != '':
                sensor.max_signal_drift = text_row[1].replace('<', '').replace(U_LESS_THAN, '').replace('%', '').replace(U_PERCENT, '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Stability')
            if text_row[0] != '':
                if 'month' in text_row[0]:
                    sensor.signal_drift_interval = '1'
                elif 'year' in text_row[0]:
                    sensor.signal_drift_interval = '12'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Anticipated')
            if text_row[1] != '':
                _expected_life = int(text_row[1][0])
                if 'years' in text_row[1]:
                    _expected_life *= 12
                sensor.expected_life = str(_expected_life)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _gas_name = parser_utils.search_text(self.pdf, ' sensor')
            if _gas_name == '':
                _gas_name = parser_utils.search_text(self.pdf, ' Sensor')
            _gas_name = ' '.join(_gas_name.split()[:-1]).replace('Electrochemical ', '')
            if _gas_name == 'Gas' or _gas_name == 'Gas Sensor':
                _gas_name = parser_utils.search_text(self.pdf, 'Electrochemical ')
                _gas_name = ' '.join(_gas_name.split()[1:])
            sensor.gasses[0].gas_name = _gas_name
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            if sensor.part_number is not None:
                sensor.gasses[0].gas_empirical = sensor.part_number.split('-')[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Measurement')
            if text_row[1] != '':
                if 'ppm' in text_row[1]:
                    sensor.gasses[0].units = 'ppm'
                else:
                    sensor.gasses[0].units = '%Vol'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Resolution')
            if text_row is not None and text_row[1] != '':
                sensor.gasses[0].resolution = text_row[1].replace(sensor.gasses[0].units, '').strip()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Measurement')
            if text_row[1] != '':
                _reading = text_row[1].replace(sensor.gasses[0].units, '').strip()
                delimiter = '~' if '~' in _reading else U_TILDE
                sensor.gasses[0].min_reading = _reading.split(delimiter)[0]
                sensor.gasses[0].max_reading = _reading.split(delimiter)[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].zero_current = None

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'detecting')
            if text_row[1] != '':
                sensor.gasses[0].overgas_limit = text_row[1].replace(sensor.gasses[0].units, '').strip()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Sensitivity')
            if text_row[1] != '':
                _sensitivity_ratio = text_row[1].replace(U_LEFT_BRACKET, '(').replace(U_RIGHT_BRACKET, ')')
                delimiter = ')' if ')' in _sensitivity_ratio else ' '
                _sensitivity_ratio = _sensitivity_ratio.split(delimiter)[1].replace(U_MICRO_SMALL, U_MICRO).strip()
                if 'mA' in _sensitivity_ratio:
                    _sensitivity_ratio = 'mA/ppm'
                if _sensitivity_ratio != '':
                    sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].sensitivity_concentration = None

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Sensitivity')
            if text_row[1] != '':
                _sensitivity = text_row[1].replace(U_LEFT_BRACKET, '(').replace(U_RIGHT_BRACKET, ')')
                delimiter = ')' if ')' in _sensitivity else ' '
                if sensor.gasses[0].units == 'ppm':
                    _sensitivity = _sensitivity.split(delimiter)[0].replace('(', '').replace(')', '')
                    base_number = float(_sensitivity.split('±')[0])
                    difference = float(_sensitivity.split('±')[1])
                    sensor.gasses[0].min_sensitivity = '{:g}'.format(base_number - difference)
                    sensor.gasses[0].max_sensitivity = '{:g}'.format(base_number + difference)
                elif sensor.gasses[0].units == '%Vol':
                    _sensitivity = _sensitivity.split(delimiter)[0].replace('(', '').replace(')', '')
                    sensor.gasses[0].min_sensitivity = '{:g}'.format(float(_sensitivity.split('~')[0].replace('(', '')))
                    sensor.gasses[0].max_sensitivity = '{:g}'.format(float(_sensitivity.split('~')[1]))
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Response')
            if text_row[0] != '':
                sensor.gasses[0].t_test_type = text_row[0].split(U_LEFT_BRACKET)[-1].replace(U_RIGHT_BRACKET, '').lower()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            text_row = parser_utils.find_row_with_substring(text_rows, 'Response')
            if text_row[1] != '':
                sensor.gasses[0].t_test_response = text_row[1].replace(U_LESS_THAN_EQUAL, '').replace(U_LESS_THAN, '').replace('S', '').strip()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].t_test_response_start = None
        sensor.gasses[0].t_test_response_end = None

        if sensor.gasses[0].gas_name == 'Gas Sensor' or sensor.gasses[0].gas_name == 'Electrochemical':
            sensor.gasses[0].gas_name = None

        return sensor.to_dict()
