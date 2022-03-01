import pdfquery

from pdfscraper.models.sensor import Sensor
from pdfscraper.models.manufacturer import Manufacturer
from pdfscraper.parsers import parser_utils

# Widths of each table column in points (1 inch = 72 pts, measured using Adobe PDF)
TABLE1 = (133.2, 126)
TABLE2 = (132.48, 126)

# Strings in datasheets representing an empty specification
NULL_VALUES = ['N.D.', '.D.', 'not recommended', 'Not required', 'Not allowed']

U_PLUS_MINUS = u'\u00b1'
U_MICRO_SMALL = '\u00b5'
U_MICRO = '\u03bc'


class MembraporParserFormat2:
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

        sensor.manufacturer = Manufacturer.MEMBRAPOR
        sensor.technology = 'EC'

        try:
            _heading = parser_utils.get_pyquery_from_text(self.pdf, 'MEASUREMENT')
            if _heading is not None:
                x0 = float(_heading.attr('x0'))
                y0 = float(_heading.attr('y0')) + 36
                x1 = float(_heading.attr('x0')) + TABLE1[0] + TABLE1[1]
                y1 = float(_heading.attr('y1')) + 51
                part_name_bbox = '{0}, {1}, {2}, {3}'.format(x0, y0, x1, y1)
                _part_number = parser_utils.search_text_in_bbox(self.pdf, part_name_bbox)
                if _part_number != '':
                    sensor.part_number = _part_number
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.alias = sensor.part_number

        sensor.active = True

        sensor.datasheet_rev = None

        try:
            _pin_count = parser_utils.search_next_column(self.pdf, 'Operation Principle', TABLE1)
            if _pin_count != '':
                sensor.pin_count = _pin_count[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _weight = parser_utils.search_next_column(self.pdf, 'Weight', TABLE2)
            if _weight != '':
                sensor.weight = _weight.split()[0].replace(',', '.')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _temp = parser_utils.search_next_column(self.pdf, 'Temperature Range', TABLE1)
            if _temp != '':
                sensor.min_temp = _temp.split()[0]
                sensor.max_temp = _temp.split()[-2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _pressure = parser_utils.search_next_column(self.pdf, 'Pressure Range', TABLE1)
            if _pressure != '':
                sensor.min_pressure = 'Atmospheric - ' + _pressure.split()[-1]
                sensor.max_pressure = 'Atmospheric + ' + _pressure.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _humidity = parser_utils.search_next_column(self.pdf, 'Humidity Range', TABLE1)
            if _humidity != '':
                sensor.min_humidity = _humidity.split()[0]
                sensor.max_humidity = _humidity.split()[3]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _load = parser_utils.search_next_column(self.pdf, 'Load Resistor', TABLE1)
            if _load != '':
                sensor.min_load = _load.split()[0]
                sensor.max_load = _load.split()[-2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _bias = parser_utils.search_next_column(self.pdf, 'Bias', TABLE1)
            if _bias != '' and _bias not in NULL_VALUES:
                sensor.bias = _bias.split()[-2].replace('+', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _max_signal_drift = parser_utils.search_next_column(self.pdf, 'Long Term Output', TABLE1, search_n_rows=2)
            if _max_signal_drift != '' and _max_signal_drift not in NULL_VALUES:
                if U_PLUS_MINUS not in _max_signal_drift:
                    sensor.max_signal_drift = ''.join(_max_signal_drift.split()[:2]).replace('<', '').replace('%', '')
                else:
                    sensor.max_signal_drift = ''.join(_max_signal_drift.split()[:1]).replace('<', '').replace(U_PLUS_MINUS, '').replace('%', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _signal_drift_interval = parser_utils.search_next_column(self.pdf, 'Long Term Output', TABLE1, search_n_rows=2)
            if _signal_drift_interval != '' and _max_signal_drift not in NULL_VALUES:
                sensor.signal_drift_interval = _signal_drift_interval.split()[-2]
                if sensor.signal_drift_interval == 'per':
                    sensor.signal_drift_interval = '1'
                if _signal_drift_interval.split()[-1] == 'years':
                    sensor.signal_drift_interval = str(int(sensor.signal_drift_interval) * 12)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _expected_life = parser_utils.search_next_column(self.pdf, 'Expected Operation Life', TABLE1)
            if _expected_life != '':
                number_of_months = int(_expected_life.split()[0]) * 12
                sensor.expected_life = str(number_of_months)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _warranty = parser_utils.search_next_column(self.pdf, 'Warranty Period', TABLE1)
            if _warranty != '':
                sensor.warranty = _warranty.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        if sensor.part_number is not None:
            sensor.gasses[0].gas_empirical = sensor.part_number.split('/')[0]

        try:
            _gas_name = parser_utils.search_text(self.pdf, 'Gas Sensor in Mini Housing')
            if _gas_name != '':
                sensor.gasses[0].gas_name = ' '.join(_gas_name.split()[:-5])
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _units = parser_utils.search_next_column(self.pdf, 'Resolution', TABLE1, search_n_rows=2)
            if _units != '':
                sensor.gasses[0].units = _units.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _resolution = parser_utils.search_next_column(self.pdf, 'Resolution', TABLE1, search_n_rows=2)
            if _resolution != '':
                sensor.gasses[0].resolution = _resolution.split()[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _reading = parser_utils.search_next_column(self.pdf, 'Nominal Range', TABLE1)
            if _reading != '':
                sensor.gasses[0].min_reading = _reading.split()[0].replace('\'', '')
                sensor.gasses[0].max_reading = _reading.split()[-2].replace('\'', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].zero_current = None

        try:
            _overgas_limit = parser_utils.search_next_column(self.pdf, 'Maximum Overload', TABLE1)
            if _overgas_limit != '' and _overgas_limit not in NULL_VALUES:
                sensor.gasses[0].overgas_limit = _overgas_limit.split()[-2].replace('\'', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _sensitivity_ratio = parser_utils.search_next_column(self.pdf, 'Output Signal', TABLE1)
            sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio.split()[-1].replace('%', 'ppm').replace(U_MICRO_SMALL, U_MICRO)
            if sensor.gasses[0].sensitivity_ratio == '1':
                sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio.split()[-2]
        except (IndexError, ValueError, TypeError, AttributeError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].sensitivity_concentration = None

        try:
            _sensitivity = parser_utils.search_next_column(self.pdf, 'Output Signal', TABLE1)
            if _sensitivity != '':
                base_number = float(_sensitivity.split()[0])
                difference = float(_sensitivity.split()[2])
                sensor.gasses[0].min_sensitivity = '{:g}'.format(base_number - difference)
                sensor.gasses[0].max_sensitivity = '{:g}'.format(base_number + difference)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test = parser_utils.search_text(self.pdf, 'Response Time')
            if _t_test != '' and _t_test not in NULL_VALUES:
                sensor.gasses[0].t_test_type = _t_test.split()[0].lower()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test_response = parser_utils.search_next_column(self.pdf, 'Response Time', TABLE1)
            if _t_test_response != '' and _t_test_response not in NULL_VALUES:
                sensor.gasses[0].t_test_response = _t_test_response.split()[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].t_test_response_start = None
        sensor.gasses[0].t_test_response_end = None

        return sensor.to_dict()
