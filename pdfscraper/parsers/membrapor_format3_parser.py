import pdfquery

from pdfscraper.models.sensor import Sensor
from pdfscraper.models.manufacturer import Manufacturer
from pdfscraper.parsers import parser_utils

# Strings in datasheets representing an empty specification
NULL_VALUES = ['not recommended', 'N.D.', 'None']

U_MICRO_SMALL = '\u00b5'
U_MICRO = '\u03bc'


class MembraporParserFormat3:
    def __init__(self, pdf):
        """Initialises the parser of Membrapor datasheets with the table like
        format.

        Args:
            pdf (PDFQuery): A query wrapper around a supplied PDF for data to
                be extracted from.
        """
        self.pdf = pdf

    def parse(self):
        """Extracts each sensor specification from a Membrapor datasheet.

        Returns:
            dict: A dictionary filled in the found values. Any values that
            could not be found will be defaulted to the None type.
        """
        sensor = Sensor()

        text_rows = parser_utils.extract_tabular_data(self.pdf, x_tolerance=3.5, y_tolerance_down=3.2, page_num=0)
        text_rows.extend(parser_utils.extract_tabular_data(self.pdf, x_tolerance=3.5, y_tolerance_down=3.2, page_num=1))
        text_rows.extend(parser_utils.extract_tabular_data(self.pdf, x_tolerance=3.5, y_tolerance_down=3.2, page_num=2))
        text_rows[:] = [row for row in text_rows if len(row) > 1 and len(''.join(row)) < 100]

        sensor.manufacturer = Manufacturer.MEMBRAPOR
        sensor.technology = 'EC'

        try:
            _part_number = parser_utils.search_text(self.pdf, ' Gas Sensor ', page_num=1)
            if _part_number != '':
                sensor.part_number = _part_number.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            if sensor.part_number is not None:
                sensor.alias = sensor.part_number
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.active = True

        sensor.datasheet_rev = None

        try:
            _pin_count = parser_utils.find_row_with_string(text_rows, 'Operation Principle')[1]
            if _pin_count[0].isdigit():
                sensor.pin_count = _pin_count[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _weight = parser_utils.find_row_with_string(text_rows, 'Weight')[1]
            sensor.weight = _weight.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _temp = parser_utils.find_row_with_string(text_rows, 'Temperature Range')[1]
            sensor.min_temp = _temp.split()[0]
            sensor.max_temp = _temp.split()[-2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _pressure = parser_utils.find_row_with_string(text_rows, 'Pressure Range')[1]
            sensor.min_pressure = 'Atmospheric - ' + _pressure.split()[-1]
            sensor.max_pressure = 'Atmospheric + ' + _pressure.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _humidity = parser_utils.find_row_with_string(text_rows, 'Relative Humidity Range')[1]
            sensor.min_humidity = _humidity.split()[0]
            sensor.max_humidity = _humidity.split()[3]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _load = parser_utils.find_row_with_substring(text_rows, 'Load Resistor')[1]
            sensor.min_load = _load.split()[0]
            sensor.max_load = _load.split()[-2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _bias = parser_utils.find_row_with_substring(text_rows, 'Bias')[1]
            if _bias not in NULL_VALUES:
                sensor.bias = _bias.split()[0].replace('+', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _drift = parser_utils.find_row_with_substring(text_rows, 'Output Drift')[1]
            sensor.max_signal_drift = _drift.split()[1].replace('%', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _drift_interval = parser_utils.find_row_with_substring(text_rows, 'Output Drift')[1]
            sensor.signal_drift_interval = _drift_interval.split()[-2]
            if sensor.signal_drift_interval == 'per':
                sensor.signal_drift_interval = '1'
            if 'year' in _drift_interval.split()[-1]:
                sensor.signal_drift_interval = str(int(sensor.signal_drift_interval) * 12)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _expected_life = parser_utils.find_row_with_substring(text_rows, 'Operation Life')[1]
            sensor.expected_life = _expected_life.split()[0]
            if 'year' in _expected_life:
                sensor.expected_life = str(int(sensor.expected_life) * 12)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _warranty = parser_utils.find_row_with_substring(text_rows, 'Warranty Period')[1]
            sensor.warranty = _warranty.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            if sensor.part_number is not None:
                sensor.gasses[0].gas_empirical = sensor.part_number.split('/')[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _gas_name = parser_utils.search_text(self.pdf, ' Gas Sensor ', page_num=1)
            sensor.gasses[0].gas_name = ' '.join(_gas_name.splitlines()[0].split()[:-3])
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _units = parser_utils.find_row_with_string(text_rows, 'Nominal Range')[1]
            if '%' not in _units:
                sensor.gasses[0].units = _units.split()[-1]
            else:
                sensor.gasses[0].units = '%Vol'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _resolution = parser_utils.find_row_with_substring(text_rows, 'Resolution')[1]
            sensor.gasses[0].resolution = _resolution.split()[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _reading = parser_utils.find_row_with_string(text_rows, 'Nominal Range')[1]
            sensor.gasses[0].min_reading = _reading.split()[0]
            sensor.gasses[0].max_reading = _reading.split()[2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].zero_current = None

        try:
            _overgas = parser_utils.find_row_with_string(text_rows, 'Maximum Overload')[1]
            sensor.gasses[0].overgas_limit = _overgas.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _sensitivity_ratio = parser_utils.find_row_with_string(text_rows, 'Output Signal')[1]
            sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio.split()[-1].replace(U_MICRO_SMALL, '&#181;').replace('%', 'ppm')
        except (IndexError, ValueError, TypeError, AttributeError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].sensitivity_concentration = None

        try:
            _sensitivity = parser_utils.find_row_with_string(text_rows, 'Output Signal')[1]
            if '-' not in _sensitivity:
                base_number = float(_sensitivity.split()[0])
            else:
                base_number = float(''.join(_sensitivity.split()[:2]))
            difference = float(_sensitivity.split()[-2])
            sensor.gasses[0].min_sensitivity = '{:g}'.format(base_number - difference)
            sensor.gasses[0].max_sensitivity = '{:g}'.format(base_number + difference)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test = parser_utils.find_row_with_substring(text_rows, 'Response Time')[0]
            sensor.gasses[0].t_test_type = _t_test.split()[0].lower()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test_response = parser_utils.find_row_with_substring(text_rows, 'Response Time')[1]
            sensor.gasses[0].t_test_response = _t_test_response.split()[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].t_test_response_start = None

        sensor.gasses[0].t_test_response_end = None

        return sensor.to_dict()
