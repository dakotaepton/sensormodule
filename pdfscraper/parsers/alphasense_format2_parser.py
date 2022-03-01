import pdfquery

from pdfscraper.models.sensor import Sensor
from pdfscraper.models.manufacturer import Manufacturer
from pdfscraper.parsers import parser_utils

# Strings in datasheets representing an empty specification
NULL_VALUES = ['nd', 'not required']

U_MICRO_SMALL = '\u00b5'
U_MICRO = '\u03bc'


class AlphasenseParserFormat2:
    def __init__(self, pdf):
        """Initialises the parser of Alphasense datasheets with the table like
        format.

        Args:
            pdf (PDFQuery): A query wrapper around a supplied PDF for data to
                be extracted from.
        """
        self.pdf = pdf

    def parse(self):
        """Extracts each sensor specification from a Alphasense datasheet.

        Returns:
            dict: A dictionary filled in the found values. Any values that
            could not be found will be defaulted to the None type.
        """
        sensor = Sensor()

        text_rows_gas_1 = parser_utils.extract_tabular_data(self.pdf, x_start=90, x_tolerance=3.5, y_tolerance_down=3.2, page_num=0)
        text_rows_gas_1[:] = [row for row in text_rows_gas_1 if len(row) > 1]

        sensor.manufacturer = Manufacturer.ALPHASENSE
        sensor.technology = 'EC'

        try:
            _part_number = parser_utils.search_text(self.pdf, 'Schematic Diagram')
            if _part_number != '':
                _part_number = _part_number.split()[2]
            else:
                _part_number = parser_utils.search_text(self.pdf, 'Doc. Ref.', page_num=3)
                _part_number = _part_number.split()[-1].split('/')[0]
            sensor.part_number = _part_number
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.alias = sensor.part_number

        sensor.active = True

        sensor.datasheet_rev = None

        try:
            _pin_count = parser_utils.search_text(self.pdf, '-Electrode')
            if _pin_count != '':
                sensor.pin_count = _pin_count.split()[-1][0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _weight = parser_utils.find_row_with_substring(text_rows_gas_1, 'Weight')[-1]
            sensor.weight = _weight.replace('<', '').strip()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _temp = parser_utils.find_row_with_substring(text_rows_gas_1, 'Temperature range')[-1].replace('+', '')
            sensor.min_temp = _temp.split()[0]
            sensor.max_temp = _temp.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _pressure = parser_utils.find_row_with_substring(text_rows_gas_1, 'Pressure range')[-1]
            sensor.min_pressure = _pressure.split()[0]
            sensor.max_pressure = _pressure.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _humidity = parser_utils.find_row_with_substring(text_rows_gas_1, 'Humidity range')[-1]
            sensor.min_humidity = _humidity.split()[0]
            sensor.max_humidity = _humidity.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _load = parser_utils.find_row_with_string(text_rows_gas_1, 'Load resistor')[-1]
            if 'to' in _load:
                sensor.min_load = _load.split()[0]
                sensor.max_load = _load.split()[-1]
            else:
                sensor.min_load = _load
                sensor.max_load = _load
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _bias = parser_utils.find_row_with_substring(text_rows_gas_1, 'Bias voltage')
            if _bias is not None and _bias[-1] not in NULL_VALUES:
                sensor.bias = _bias[-1].replace('+', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _drift = parser_utils.find_row_with_substring(text_rows_gas_1, 'Output drift')[-1]
            if _drift not in NULL_VALUES:
                sensor.max_signal_drift = _drift.replace('<', '').strip()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _expected_life = parser_utils.find_row_with_substring(text_rows_gas_1, 'Operating life')[-1]
            if _expected_life not in NULL_VALUES:
                sensor.expected_life = _expected_life.replace('>', '').strip()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            if _drift not in NULL_VALUES:
                _drift_interval = parser_utils.find_row_with_substring(text_rows_gas_1, 'Output drift')[1]
                sensor.signal_drift_interval = _drift_interval.split()[-2]
                if 'year' in _drift_interval:
                    sensor.signal_drift_interval *= '12'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _gas_name = parser_utils.search_text(self.pdf, ' Sensor')
            sensor.gasses[0].gas_name = ' '.join(_gas_name.split(' Sensor')[0].split()[1:])
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _gas_empirical = parser_utils.find_row_with_substring(text_rows_gas_1, 'Response time')[1]
            sensor.gasses[0].gas_empirical = _gas_empirical.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _units = parser_utils.find_row_with_substring(text_rows_gas_1, 'Output')[1]
            if '%' in _units:
                sensor.gasses[0].units = '%Vol'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            sensor.gasses[0].min_reading = '0'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _zero_current = parser_utils.find_row_with_substring(text_rows_gas_1, 'Zero current')[-1]
            sensor.gasses[0].zero_current = ' '.join(_zero_current.split())
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _sensitivity_ratio = parser_utils.find_row_with_string(text_rows_gas_1, 'Output')
            _sensitivity_ratio = _sensitivity_ratio[2] if _sensitivity_ratio[0] == 'PERFORMANCE' else _sensitivity_ratio[1]
            sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio.split()[0].replace(U_MICRO_SMALL, U_MICRO)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _sensitivity_conc = parser_utils.find_row_with_string(text_rows_gas_1, 'Output')
            _sensitivity_conc = _sensitivity_conc[2] if _sensitivity_conc[0] == 'PERFORMANCE' else _sensitivity_conc[1]
            sensor.gasses[0].sensitivity_concentration = _sensitivity_conc.split()[2].split('%')[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _sensitivity = parser_utils.find_row_with_string(text_rows_gas_1, 'Output')[-1]
            sensor.gasses[0].min_sensitivity = _sensitivity.split()[0]
            sensor.gasses[0].max_sensitivity = _sensitivity.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test = parser_utils.find_row_with_substring(text_rows_gas_1, 'Response time')[1]
            sensor.gasses[0].t_test_type = _t_test.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test_response = parser_utils.find_row_with_substring(text_rows_gas_1, 'Response time')[-1]
            sensor.gasses[0].t_test_response = _t_test_response.replace('<', '').strip()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test_start = parser_utils.find_row_with_substring(text_rows_gas_1, 'Response time')[1]
            sensor.gasses[0].t_test_response_start = _t_test_start.split()[3].replace('%', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test_end = parser_utils.find_row_with_substring(text_rows_gas_1, 'Response time')[1]
            sensor.gasses[0].t_test_response_end = _t_test_end.split()[5].replace('%', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        if 'Ltd,' in sensor.gasses[0].gas_name:
            sensor.gasses[0].gas_name = None

        return sensor.to_dict()
