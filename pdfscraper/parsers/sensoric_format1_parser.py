import pdfquery

from pdfscraper.models.sensor import Sensor
from pdfscraper.models.manufacturer import Manufacturer
from pdfscraper.parsers import parser_utils
from pdfscraper.parsers.parser_utils import Justification

# Widths of each table column in points (1 inch = 72 pts, measured using Adobe PDF)
TABLE = (155.52, 167.76)

# Strings in datasheets representing an empty specification
NULL_VALUES = ['not required', '0 v', 'none']


class SensoricParserFormat1:
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

        # Which page to search for gas sensor specifications
        title_page = parser_utils.search_text(self.pdf, 'Document Purpose', page_num=None)
        page_num = 0 if title_page is '' else 1

        # The x coordinate of the main divide between the left and right side of the datasheet
        split = parser_utils.get_pyquery_from_text(self.pdf, 'non-condensing', page_num=page_num)
        split = float(split.attr('x1')) + 15

        text_rows = parser_utils.extract_tabular_data(self.pdf, x_end=split-10, x_tolerance=3, y_tolerance_down=5, page_num=page_num)
        text_rows.extend(parser_utils.extract_tabular_data(self.pdf, x_start=split-5, x_tolerance=3, page_num=page_num))
        text_rows[:] = [row for row in text_rows if len(row) > 1]

        sensor.manufacturer = Manufacturer.SENSORIC
        sensor.technology = 'EC'

        try:
            _part_number = parser_utils.find_row_with_string(text_rows, '4-Series')
            if _part_number is None:
                _part_number = parser_utils.find_row_with_string(text_rows, '4 Series')
            sensor.part_number = _part_number[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _alias = parser_utils.search_text(self.pdf, 'Sensoric ', page_num=page_num)
            sensor.alias = ' '.join(_alias.splitlines()[0].split()[1:])
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.active = True

        sensor.datasheet_rev = None

        try:
            _pin_count = parser_utils.find_row_with_string(text_rows, 'Operating Principle')[1]
            if _pin_count[0].isdigit():
                sensor.pin_count = _pin_count[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _weight = parser_utils.find_row_with_string(text_rows, 'Weight')[1]

            # Datasheet shows weights for multiple containers
            if ':' in _weight or '(' in _weight:
                index = 2
                _weight = parser_utils.search_next_column(self.pdf, 'Weight', TABLE, justification=Justification.RIGHT, search_n_rows=index, page_num=page_num)
                while '4 series' not in _weight.replace('-', ' ').lower():
                    index += 1
                    _weight = parser_utils.search_next_column(self.pdf, 'Weight', TABLE, justification=Justification.RIGHT, search_n_rows=index, page_num=page_num)
                _weight = ' '.join(_weight.replace('-', ' ').replace(':', ' ').split()[-4:])

            sensor.weight = '{:g}'.format(float(_weight.split()[0]))
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _temp = parser_utils.find_row_with_substring(text_rows, 'Temperature Range')
            if _temp is None:
                _temp = parser_utils.find_row_with_string(text_rows, 'Continuous')
            _temp = _temp[1].replace('°C', '').replace('+', '')
            sensor.min_temp = _temp.split()[0]
            sensor.max_temp = _temp.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _pressure = parser_utils.find_row_with_substring(text_rows, 'Pressure Range')[1]
            sensor.min_pressure = 'Atmospheric - ' + _pressure.split()[-1]
            sensor.max_pressure = 'Atmospheric + ' + _pressure.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _humidity = parser_utils.find_row_with_substring(text_rows, 'Humidity Range')[1].replace('%', '')
            sensor.min_humidity = _humidity.split()[0]
            sensor.max_humidity = _humidity.split()[2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _load = parser_utils.find_row_with_substring(text_rows, 'Load Resistor')[1]
            if _load.lower() not in NULL_VALUES:
                sensor.min_load = _load.split()[0]
                sensor.max_load = _load.split()[0]
                if 'k' in _load:
                    sensor.min_load = '{:g}'.format(float(sensor.min_load) * 1000)
                    sensor.max_load = '{:g}'.format(float(sensor.max_load) * 1000)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _bias = parser_utils.find_row_with_string(text_rows, 'Bias Voltage')[1]
            if _bias.lower() not in NULL_VALUES:
                sensor.bias = _bias.split()[0].replace('+', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _drift = parser_utils.find_row_with_substring(text_rows, 'Output Drift')[1]
            sensor.max_signal_drift = _drift.split()[-4].replace('<', '').replace('%', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _drift_interval = parser_utils.find_row_with_substring(text_rows, 'Output Drift')[1]
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
            _gas_name = parser_utils.search_text(self.pdf, ' Gas Sensor', page_num=page_num)
            _gas_name = _gas_name.splitlines()[0]
            if 'filter' in _gas_name.lower():
                _gas_name = _gas_name[:-3]
            if '(' in _gas_name:
                sensor.gasses[0].gas_name = _gas_name.split('(')[0].strip()
            else:
                sensor.gasses[0].gas_name = ' '.join(_gas_name.split()[:-2])
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            sensor.gasses[0].gas_empirical = sensor.alias.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _units = parser_utils.find_row_with_string(text_rows, 'Measurement Range')[1]
            sensor.gasses[0].units = _units.split()[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _resolution = parser_utils.find_row_with_string(text_rows, 'Resolution')[1]
            if 'dependent' in _resolution.lower():
                _resolution = parser_utils.search_next_column(self.pdf, 'Resolution', TABLE, justification=Justification.RIGHT, search_n_rows=2, page_num=page_num)
                sensor.gasses[0].resolution = _resolution.split()[-4].replace('<', '')
            else:
                sensor.gasses[0].resolution = _resolution.replace('<', '').strip().split()[0]
            if 'ppb' in _resolution:
                sensor.gasses[0].resolution = '{:g}'.format(float(sensor.gasses[0].resolution) / 1000)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _reading = parser_utils.find_row_with_string(text_rows, 'Measurement Range')[1]
            sensor.gasses[0].min_reading = _reading.split()[0].split('-')[0]
            sensor.gasses[0].max_reading = _reading.split()[0].split('-')[1]

            _min_reading = parser_utils.find_row_with_substring(text_rows, 'Detection Limit')
            if _min_reading is not None:
                sensor.gasses[0].min_reading = _min_reading[1].replace('<', '').strip().split()[0]
                if 'ppb' in _min_reading[1]:
                    sensor.gasses[0].min_reading = '{:g}'.format(float(sensor.gasses[0].min_reading) / 1000)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _zero_current = parser_utils.find_row_with_substring(text_rows, 'Baseline Offset')[1]
            sensor.gasses[0].zero_current = _zero_current.split()[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _overgas = parser_utils.find_row_with_substring(text_rows, 'Overload')[1]
            sensor.gasses[0].overgas_limit = _overgas.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _sensitivity_ratio = parser_utils.find_row_with_substring(text_rows, 'Sensitivity')[1]
            sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio.split()[3]
        except (IndexError, ValueError, TypeError, AttributeError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].sensitivity_concentration = None

        try:
            _sensitivity = parser_utils.find_row_with_substring(text_rows, 'Sensitivity')[1]
            base_number = float(_sensitivity.split()[0])
            difference = float(_sensitivity.split()[2])
            sensor.gasses[0].min_sensitivity = '{:g}'.format(base_number - difference)
            sensor.gasses[0].max_sensitivity = '{:g}'.format(base_number + difference)

            if 'negative' in _sensitivity:
                sensor.gasses[0].min_sensitivity, sensor.gasses[0].max_sensitivity = '-' + sensor.gasses[0].max_sensitivity, '-' + sensor.gasses[0].min_sensitivity
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test = parser_utils.search_text(self.pdf, 'Response Time', page_num=page_num)
            sensor.gasses[0].t_test_type = _t_test.split()[-1].replace('(', '').replace(')', '').replace('*', '').lower()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test_response = parser_utils.search_next_column(self.pdf, 'Response Time', TABLE, justification=Justification.RIGHT, page_num=page_num)
            sensor.gasses[0].t_test_response = _t_test_response.replace('<', '').strip().split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].t_test_response_start = None

        sensor.gasses[0].t_test_response_end = None

        return sensor.to_dict()
