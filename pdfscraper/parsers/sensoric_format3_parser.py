import pdfquery

from pdfscraper.models.sensor import Sensor
from pdfscraper.models.manufacturer import Manufacturer
from pdfscraper.parsers import parser_utils
from pdfscraper.parsers.parser_utils import Justification

# Widths of each table column in points (1 inch = 72 pts, measured using Adobe PDF)
TABLE = (155.52, 167.76)

# Strings in datasheets representing an empty specification
NULL_VALUES = ['not required', '0 v', '0v', '0mv', '0 mv', 'none']

U_OHM = '\u2126'


class SensoricParserFormat3:
    def __init__(self, pdf):
        """Initialises the parser of Sensoric datasheets with the table like
        format.

        Args:
            pdf (PDFQuery): A query wrapper around a supplied PDF for data to
                be extracted from.
        """
        self.pdf = pdf

    def parse(self):
        """Extracts each sensor specification from a Sensoric datasheet.

        Returns:
            dict: A dictionary filled in the found values. Any values that
            could not be found will be defaulted to the None type.
        """
        sensor = Sensor()

        # The x coordinate of the main divide between the left and right side of the datasheet
        split = parser_utils.get_pyquery_from_text(self.pdf, 'non-condensing')
        split = float(split.attr('x1')) + 15

        text_rows = parser_utils.extract_tabular_data(self.pdf, x_end=split-10, x_tolerance=4, y_tolerance=4)
        text_rows.extend(parser_utils.extract_tabular_data(self.pdf, x_start=split-5, x_tolerance=4))

        sensor.manufacturer = Manufacturer.SENSORIC
        sensor.technology = 'EC'

        try:
            _part_number = parser_utils.find_row_with_substring(text_rows, '4 Series')
            sensor.part_number = _part_number[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _alias = parser_utils.search_text(self.pdf, 'Sensoric ')
            sensor.alias = ' '.join(_alias.splitlines()[0].split()[1:])
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.active = True

        sensor.datasheet_rev = None

        try:
            _pin_count = parser_utils.search_text(self.pdf, 'electrode')
            if _pin_count.split()[2].isdigit():
                sensor.pin_count = _pin_count.split()[2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            # Weight
            pass
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _temp = parser_utils.find_row_with_string(text_rows, 'Temperature Range')
            _temp = _temp[1].replace('°C', '').replace('+', '')
            sensor.min_temp = _temp.split(' to ')[0]
            sensor.max_temp = _temp.split(' to ')[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            # Pressure
            pass
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _humidity = parser_utils.find_row_with_string(text_rows, 'Operating Humidity')[1].replace('%', '')
            sensor.min_humidity = _humidity.split()[0].split('-')[0]
            sensor.max_humidity = _humidity.split()[0].split('-')[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _load = parser_utils.find_row_with_substring(text_rows, 'Resistor')[1]
            if _load.lower() not in NULL_VALUES:
                sensor.min_load = _load.replace('k', '').replace(U_OHM, '').split('-')[0]
                sensor.max_load = _load.replace('k', '').replace(U_OHM, '').split('-')[-1]
                if 'k' in _load:
                    sensor.min_load = '{:g}'.format(float(sensor.min_load) * 1000)
                    sensor.max_load = '{:g}'.format(float(sensor.max_load) * 1000)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _bias = parser_utils.find_row_with_string(text_rows, 'Bias Potential')[1]
            if _bias.lower() not in NULL_VALUES:
                sensor.bias = _bias.split()[0].replace('+', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _drift = parser_utils.find_row_with_substring(text_rows, 'Sensitivity Drift')[1]
            sensor.max_signal_drift = _drift.split()[0].replace('<', '').replace('%', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _drift_interval = parser_utils.find_row_with_substring(text_rows, 'Sensitivity Drift')[1]
            sensor.signal_drift_interval = _drift_interval.split()[-2]
            if 'year' in _drift_interval:
                sensor.signal_drift_interval = str(int(sensor.signal_drift_interval) * 12)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _expected_life = parser_utils.find_row_with_substring(text_rows, 'Operating Life')[1]
            sensor.expected_life = _expected_life.replace('>', '').strip().split()[0]
            if 'year' in _expected_life:
                sensor.expected_life = str(int(sensor.expected_life) * 12)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _warranty = parser_utils.find_row_with_substring(text_rows, 'Warranty')[1]
            sensor.warranty = _warranty.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            # Gas name
            pass
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            sensor.gasses[0].gas_empirical = sensor.alias.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _units = parser_utils.find_row_with_string(text_rows, 'Measuring Range')[1]
            if 'ppm' in _units:
                sensor.gasses[0].units = 'ppm'
            elif '%' in _units:
                sensor.gasses[0].units = '%Vol'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            # Resolution
            pass
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _reading = parser_utils.find_row_with_string(text_rows, 'Measuring Range')[1].replace(sensor.gasses[0].units, '')
            sensor.gasses[0].min_reading = _reading.split('-')[0]
            sensor.gasses[0].max_reading = _reading.split('-')[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _zero_current = parser_utils.find_row_with_substring(text_rows, 'Zero Current')[1].replace('<', '')
            sensor.gasses[0].zero_current = ''.join(_zero_current.split()[:2]).replace('nA', '').replace(',', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            # Overgas
            pass
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _sensitivity_ratio = parser_utils.find_row_with_substring(text_rows, 'Sensitivity Range')[1]
            if 'ppm' in _sensitivity_ratio:
                sensor.gasses[0].sensitivity_ratio = 'nA/ppm'
        except (IndexError, ValueError, TypeError, AttributeError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].sensitivity_concentration = None

        try:
            _sensitivity = parser_utils.find_row_with_substring(text_rows, 'Sensitivity Range')[1].replace(sensor.gasses[0].sensitivity_ratio, '')
            base_number = float(_sensitivity.split()[0])
            difference = float(_sensitivity.split()[2])
            sensor.gasses[0].min_sensitivity = '{:g}'.format(base_number - difference)
            sensor.gasses[0].max_sensitivity = '{:g}'.format(base_number + difference)

            if 'negative' in _sensitivity:
                sensor.gasses[0].min_sensitivity, sensor.gasses[0].max_sensitivity = '-' + sensor.gasses[0].max_sensitivity, '-' + sensor.gasses[0].min_sensitivity
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test = parser_utils.find_row_with_substring(text_rows, 'Response Time')
            _t_test = text_rows[text_rows.index(_t_test) + 3][0]
            sensor.gasses[0].t_test_type = _t_test.replace('<', '').strip().split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            sensor.gasses[0].t_test_response = _t_test.replace('<', '').replace('s', '').strip().split()[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].t_test_response_start = None

        sensor.gasses[0].t_test_response_end = None

        return sensor.to_dict()
