import copy

import pdfquery

from pdfscraper.models.sensor import Sensor
from pdfscraper.models.manufacturer import Manufacturer
from pdfscraper.parsers import parser_utils
from pdfscraper.parsers.parser_utils import Justification

# Widths of each table column in points (1 inch = 72 pts, measured using Adobe PDF)
TABLE = (212.40, 360.00)

# Strings in datasheets representing an empty specification
NULL_VALUES = ['not required', '0 mv', '0 v']

U_EN_DASH = '\u2013'
U_PLUS_MINUS = '\u00b1'


class SensoricParserFormat2:
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

        text_rows = parser_utils.extract_tabular_data(self.pdf, x_tolerance=5, y_tolerance_down=2, page_num=0)
        text_rows.extend(parser_utils.extract_tabular_data(self.pdf, x_tolerance=5, y_tolerance_down=2, page_num=1))
        text_rows.extend(parser_utils.extract_tabular_data(self.pdf, x_tolerance=5, y_tolerance_down=2, page_num=2))

        sensor.manufacturer = Manufacturer.SENSORIC
        sensor.technology = 'EC'

        try:
            _part_number = parser_utils.find_row_with_substring(text_rows, '4 series')
            sensor.part_number = _part_number[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _alias = parser_utils.search_text(self.pdf, 'Sensoric ', page_num=1)
            sensor.alias = ' '.join(_alias.splitlines()[0-1].split()[1:])
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.active = True

        sensor.datasheet_rev = None

        try:
            _pin_count = parser_utils.find_row_with_substring(text_rows, 'electrode ')[0]
            if _pin_count.split()[1][0].isdigit():
                sensor.pin_count = _pin_count.split()[1][0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            # Weight
            pass
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _temp = parser_utils.find_row_with_substring(text_rows, 'Temperature Range')[1].replace('°C', '').replace('+', '')
            sensor.min_temp = _temp.split()[0]
            sensor.max_temp = _temp.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            # Pressure
            pass
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _humidity = parser_utils.find_row_with_substring(text_rows, 'Humidity Range')[1].replace('%', '').replace(U_EN_DASH, '-')
            if ' -' in _humidity:
                _humidity = _humidity.replace(' -', '-')

            sensor.min_humidity = _humidity.split()[0].split('-')[0]
            sensor.max_humidity = _humidity.split()[0].split('-')[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            # Load
            pass
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _bias = parser_utils.find_row_with_string(text_rows, 'Bias Potential')[1]
            if _bias.lower() not in NULL_VALUES:
                sensor.bias = _bias.split()[0].replace('+', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _drift = parser_utils.find_row_with_substring(text_rows, 'Sensitivity Drift')[1].replace('<', '').replace('%', '').strip()
            sensor.max_signal_drift = _drift.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _drift_interval = parser_utils.find_row_with_substring(text_rows, 'Sensitivity Drift')[1]
            sensor.signal_drift_interval = _drift_interval.split()[-2]
            if sensor.signal_drift_interval.lower() == 'per':
                sensor.signal_drift_interval = '1'
            if 'year' in _drift_interval:
                sensor.signal_drift_interval = str(int(sensor.signal_drift_interval) * 12)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _expected_life = parser_utils.find_row_with_substring(text_rows, 'Life Expectancy')[1].replace('>', '').strip()
            sensor.expected_life = _expected_life.split()[0]
            if 'year' in _expected_life:
                sensor.expected_life = str(int(sensor.expected_life) * 12)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _warranty = parser_utils.find_row_with_substring(text_rows, 'Warranty')[1]
            sensor.warranty = _warranty.split()[0]
            if 'year' in _warranty:
                sensor.warranty = str(int(sensor.warranty) * 12)
            elif 'week' in _warranty:
                sensor.warranty = str(int(sensor.warranty) // 4)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            sensor.gasses[0].gas_name = text_rows[0][0].replace('Sensor', '').strip()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            sensor.gasses[0].gas_empirical = sensor.alias.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _units = parser_utils.find_row_with_substring(text_rows, 'Resolution')[1]
            sensor.gasses[0].units = _units.split()[-1]
            if sensor.gasses[0].units == 'ppb':
                sensor.gasses[0].units = 'ppm'
            if 'mg' in sensor.gasses[0].units:
                sensor.gasses[0].units = 'mg/m^3'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _resolution = parser_utils.find_row_with_substring(text_rows, 'Resolution')[1]
            if 'typically' in _resolution:
                _resolution = ' '.join(_resolution.split()[:-3])

            sensor.gasses[0].resolution = _resolution.replace('<', '').strip().split()[0]
            if 'ppb' in _resolution:
                sensor.gasses[0].resolution = '{:g}'.format(float(sensor.gasses[0].resolution) / 1000)

            if sensor.gasses[0].units == 'mg/m^3':
                sensor.gasses[0].resolution = '{:g}'.format(round(float(sensor.gasses[0].resolution) / 3.66, 2))
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _reading = parser_utils.find_row_with_string(text_rows, 'Measuring Range')[1].replace(U_EN_DASH, '-').replace('-', ' ')
            if 'typically' in _reading:
                _reading = ' '.join(_reading.split()[-3:])

            sensor.gasses[0].min_reading = _reading.split()[0]
            sensor.gasses[0].max_reading = _reading.split()[1]

            if '%' in sensor.gasses[0].max_reading:
                sensor.gasses[0].max_reading = '{:g}'.format(float(sensor.gasses[0].max_reading.replace('%', '')) / 100 * 1E6)
            elif 'ppb' in _reading:
                sensor.gasses[0].min_reading = '{:g}'.format(float(sensor.gasses[0].min_reading) / 1000)
                sensor.gasses[0].max_reading = '{:g}'.format(float(sensor.gasses[0].max_reading) / 1000)

            if sensor.gasses[0].units == 'mg/m^3':
                sensor.gasses[0].min_reading = '{:g}'.format(round(float(sensor.gasses[0].min_reading) / 3.66))
                sensor.gasses[0].max_reading = '{:g}'.format(round(float(sensor.gasses[0].max_reading) / 3.66))
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _zero_current = parser_utils.find_row_with_substring(text_rows, 'Zero Current')[1]
            sensor.gasses[0].zero_current = ''.join(_zero_current.replace('<', '').strip().split()[:1])
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _overgas = parser_utils.find_row_with_string(text_rows, 'Measuring Range')[1].replace(U_EN_DASH, '-')
            if 'typically' in _overgas:
                sensor.gasses[0].overgas_limit = _overgas.split()[0].split('-')[1]
            elif sensor.gasses[0].overgas_limit is None and '/' in _overgas:
                sensor.gasses[0].overgas_limit = _overgas.split('/')[1].split()[0].split('-')[1]

            if sensor.gasses[0].overgas_limit is None:
                _overgas = parser_utils.find_row_with_substring(text_rows, 'short gas exposure')
                if _overgas is not None:
                    _overgas = _overgas[0].replace(U_EN_DASH, '-')
                    sensor.gasses[0].overgas_limit = _overgas.replace('-', ' ').split()[2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _sensitivity_ratio = parser_utils.find_row_with_substring(text_rows, 'Sensitivity')[1].replace('+/-', '').replace('/ ', '/').replace(U_EN_DASH, '-')
            if '-' in _sensitivity_ratio:
                sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio.replace('-', ' ').split()[2]
            else:
                index = 2 if _sensitivity_ratio.split()[1].isdigit() else 3
                sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio.split()[index]

            if 'mg' in sensor.gasses[0].sensitivity_ratio:
                sensor.gasses[0].sensitivity_ratio = sensor.gasses[0].sensitivity_ratio.split('/')[0] + '/mg/m^3'
        except (IndexError, ValueError, TypeError, AttributeError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].sensitivity_concentration = None

        try:
            _sensitivity = parser_utils.find_row_with_substring(text_rows, 'Sensitivity')[1]
            _sensitivity = _sensitivity.replace(U_EN_DASH, '-').replace('+/-', '').replace(U_PLUS_MINUS, '').replace('/ ', '/')
            if '-' in _sensitivity:
                _sensitivity = _sensitivity.replace('-', ' ')
                sensor.gasses[0].min_sensitivity = _sensitivity.split()[0]
                sensor.gasses[0].max_sensitivity = _sensitivity.split()[1]
            else:
                base_number = float(_sensitivity.split()[0])
                index = 2 if _sensitivity.split()[2].replace('.', '', 1).isdigit() else 1
                difference = float(_sensitivity.split()[index])

                sensor.gasses[0].min_sensitivity = '{:g}'.format(base_number - difference)
                sensor.gasses[0].max_sensitivity = '{:g}'.format(base_number + difference)

            if 'negative' in _sensitivity:
                sensor.gasses[0].min_sensitivity, sensor.gasses[0].max_sensitivity = '-' + sensor.gasses[0].max_sensitivity, '-' + sensor.gasses[0].min_sensitivity
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test = parser_utils.find_row_with_substring(text_rows, 'Response Time')
            num_rows_down = 2
            if ')' in text_rows[text_rows.index(_t_test) + 1][0]:
                num_rows_down += 1
            if ')' in text_rows[text_rows.index(_t_test) + num_rows_down][0]:
                num_rows_down += 1
            _t_test = text_rows[text_rows.index(_t_test) + num_rows_down]

            # For some reason this text has each character repeated. So we remove every second character from the string
            if len(_t_test[0]) > 3:
                _t_test[0] = ''.join([v for i, v in enumerate(_t_test[0]) if i % 2 == 0])
                _t_test[1] = ''.join([v for i, v in enumerate(_t_test[1]) if i % 2 == 0])
            sensor.gasses[0].t_test_type = _t_test[0].lower()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            sensor.gasses[0].t_test_response = _t_test[1].replace('<', '').strip().split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].t_test_response_start = None

        sensor.gasses[0].t_test_response_end = None

        if sensor.gasses[0].units == 'mg/m^3':
            sensor.gasses[0].units = 'ppm'
            sensor.gasses[0].sensitivity_ratio = sensor.gasses[0].sensitivity_ratio.split('/')[0] + '/ppm'

        dual_gas = parser_utils.find_row_with_substring(text_rows, '1:1 cross interference')
        if dual_gas is not None:
            sensor.add_gas_dict()
            sensor.gas_count = 2
            sensor.gasses[1] = copy.deepcopy(sensor.gasses[0])
            sensor.gasses[1].gas_name = None
            sensor.gasses[1].gas_empirical = dual_gas[0].split()[-1]

        return sensor.to_dict()
