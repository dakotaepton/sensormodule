import pdfquery

from pdfscraper.models.sensor import Sensor
from pdfscraper.models.manufacturer import Manufacturer
from pdfscraper.parsers import parser_utils

# Widths of each table column in points (1 inch = 72 pts, measured using Adobe PDF)
TABLE1 = (151.2, 116.64)
TABLE2 = (130.0, 113.76)

# Strings in datasheets representing an empty specification
NULL_VALUES = ['n.d.', 'nd', 'not required', 'not recommended', 'not allowed', 'no data']

# Unicode characters to be replaced
U_DEGREES_CELCIUS = u'\u00b0C'
U_APOSTROPHE = u'\u2019'
U_PLUS_MINUS = '\u00b1'


class MembraporParserFormat1:
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
            _part_number = parser_utils.search_text(self.pdf, 'TYPE ')
            if _part_number == '':
                _part_number = parser_utils.search_text(self.pdf, 'Type ')
            sensor.part_number = _part_number.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.alias = sensor.part_number
        sensor.active = True
        sensor.datasheet_rev = None

        try:
            if sensor.part_number != '' or sensor.part_number is not None:
                if sensor.part_number.split('-')[-1] in ['2E', '2EG']:
                    sensor.pin_count = '2'
                else:
                    sensor.pin_count = '3'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _weight = parser_utils.search_next_column(self.pdf, 'Weight', TABLE2)
            if _weight != '':
                sensor.weight = _weight.split()[1].replace(',', '.')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _temp = parser_utils.search_next_column(self.pdf, 'Temperature Range', TABLE1)
            if _temp != '':
                sensor.min_temp = ''.join(_temp.split()[0:2]).replace(U_DEGREES_CELCIUS, '')
                sensor.max_temp = ''.join(_temp.split()[-2:-1])
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _pressure = parser_utils.search_next_column(self.pdf, 'Pressure Range', TABLE1)
            if _pressure != '':
                if _pressure.split()[-2].replace('%', '') in ['\uf0b1', '\u00b1', '\u2013']:
                    sensor.min_pressure = 'Atmospheric - ' + _pressure.split()[-1].replace('%', '')
                    sensor.max_pressure = 'Atmospheric + ' + _pressure.split()[-1].replace('%', '')
                elif _pressure.split()[-2].replace('%', '') != 'Atmospheric':
                    sensor.min_pressure = 'Atmospheric - ' + _pressure.split()[-2].replace('%', '')
                    sensor.max_pressure = 'Atmospheric + ' + _pressure.split()[-2].replace('%', '')
                else:
                    sensor.min_pressure = 'Atmospheric - ' + '0'
                    sensor.max_pressure = 'Atmospheric + ' + '0'
                sensor.min_pressure += '%'
                sensor.max_pressure += '%'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _humidity = parser_utils.search_next_column(self.pdf, 'Humidity Range', TABLE1)
            if _humidity != '':
                sensor.min_humidity = _humidity.split()[0].replace('%', '')
                if _humidity.split()[4] == '%':
                    sensor.max_humidity = _humidity.split()[3]
                else:
                    sensor.max_humidity = _humidity.split()[2].replace('%', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _load = parser_utils.search_next_column(self.pdf, 'Recommended Load Resistor', TABLE1)
            if _load != '':
                sensor.min_load = _load.split()[0]
                sensor.max_load = _load.split()[-2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _bias = parser_utils.search_next_column(self.pdf, 'Bias Voltage', TABLE1)
            if _bias != '' and _bias.lower() not in NULL_VALUES:
                sensor.bias = _bias.split()[-2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _max_signal_drift = parser_utils.search_next_column(self.pdf, 'Long Term Output', TABLE1)
            if _max_signal_drift != '':
                if _max_signal_drift.lower() not in NULL_VALUES and _max_signal_drift.split()[0].lower() not in NULL_VALUES:
                    sensor.max_signal_drift = _max_signal_drift.split()[1].replace('%', '').replace(U_PLUS_MINUS, '')
            else:
                _max_signal_drift = parser_utils.search_text(self.pdf, 'Long Term Output')
                if _max_signal_drift != '' and _max_signal_drift.split()[5] not in NULL_VALUES:
                    sensor.max_signal_drift = _max_signal_drift.split()[6].replace('%', '').replace(U_PLUS_MINUS, '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _signal_drift_interval = parser_utils.search_next_column(self.pdf, 'Long Term Output', TABLE1, search_n_rows=2)
            if _signal_drift_interval != '' and _signal_drift_interval.split()[0] not in NULL_VALUES:
                if _signal_drift_interval.split()[-1] == 'Ohm':
                    _signal_drift_interval = parser_utils.search_next_column(self.pdf, 'Long Term Output', TABLE1)
            if _signal_drift_interval != '':
                if _signal_drift_interval.lower() not in NULL_VALUES and _signal_drift_interval.split()[0].lower() not in NULL_VALUES:
                    if not _signal_drift_interval.split()[-2].isdigit():
                        sensor.signal_drift_interval = _signal_drift_interval.split()[-1]
                        if sensor.signal_drift_interval in ['month', '/month', 'loss/month']:
                            sensor.signal_drift_interval = '1'
                        if sensor.signal_drift_interval == 'months':
                            sensor.signal_drift_interval = _signal_drift_interval.split()[-2]
                    else:
                        sensor.signal_drift_interval = _signal_drift_interval.split()[-2]
                        if _signal_drift_interval.split()[-1] == 'years':
                            sensor.signal_drift_interval = str(int(sensor.signal_drift_interval) * 12)
            else:
                sensor.signal_drift_interval = parser_utils.search_text(self.pdf, 'Long Term Output').split()[-2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _expected_life = parser_utils.search_next_column(self.pdf, 'Expected Operation Life', TABLE1).replace('>', '').replace('1)', '').strip()
            if _expected_life != '':
                number_of_months = int(_expected_life.split()[-4]) * 12
                sensor.expected_life = str(number_of_months)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _warranty = parser_utils.search_next_column(self.pdf, 'Warranty Period', TABLE2)
            if _warranty != '':
                sensor.warranty = _warranty.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        if sensor.part_number is not None:
            sensor.gasses[0].gas_empirical = sensor.part_number.split('/')[0]

        try:
            _gas_name = parser_utils.search_text(self.pdf, 'SPECIFICATION SHEET FOR')
            if _gas_name != '':
                sensor.gasses[0].gas_name = ''.join(_gas_name.split()[3:-3]).title()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _units = parser_utils.search_next_column(self.pdf, 'Resolution', TABLE1)
            if _units != '':
                sensor.gasses[0].units = _units.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _resolution = parser_utils.search_next_column(self.pdf, 'Resolution', TABLE1)
            if _resolution != '':
                sensor.gasses[0].resolution = _resolution.split()[0].replace(',', '.')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _reading = parser_utils.search_next_column(self.pdf, 'Nominal Range', TABLE1)
            if _reading != '':
                if '%' in _reading:
                    _reading = parser_utils.search_next_column(self.pdf, 'Nominal Range', TABLE1, search_n_rows=2).split()[3:]
                    _reading = ' '.join(_reading)
                sensor.gasses[0].min_reading = _reading.split()[0]
                sensor.gasses[0].max_reading = _reading.split()[-2].replace(U_APOSTROPHE, '').replace('\'', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].zero_current = None

        try:
            _overgas_limit = parser_utils.search_next_column(self.pdf, 'Maximum Overload', TABLE1)
            if _overgas_limit != '' and _overgas_limit.lower() not in NULL_VALUES:
                if '%' in _overgas_limit:
                    sensor.gasses[0].overgas_limit = str(int(_overgas_limit.replace('%', '')) * int(sensor.gasses[0].max_reading))
                else:
                    sensor.gasses[0].overgas_limit = _overgas_limit.split()[-2].replace(U_APOSTROPHE, '').replace('\'', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _sensitivity_ratio = parser_utils.search_next_column(self.pdf, 'Output Signal', TABLE1)
            if '%' in _sensitivity_ratio:
                _sensitivity_ratio = parser_utils.search_next_column(self.pdf, 'Output Signal', TABLE1, search_n_rows=2)
                sensor.gasses[0].sensitivity_ratio = ''.join(_sensitivity_ratio.split('%')[1].split()[-3:])
            else:
                sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].sensitivity_concentration = None

        try:
            _sensitivity = parser_utils.search_next_column(self.pdf, 'Output Signal', TABLE1).replace('*', '').replace(U_APOSTROPHE, '').replace('\'', '')
            if _sensitivity != '':
                if '%' in _sensitivity:
                    _sensitivity = parser_utils.search_next_column(self.pdf, 'Output Signal', TABLE1, search_n_rows=2)
                    sensor.gasses[0].min_sensitivity = _sensitivity.split('%')[1].split()[0]
                    sensor.gasses[0].max_sensitivity = _sensitivity.split('%')[1].split()[0]
                else:
                    base_number = float(_sensitivity.split()[-4])
                    difference = float(_sensitivity.split()[-2])

                    multiplier = 1
                    if _sensitivity.split()[0] == '-':  # If a negative sign is not connected to the number (manually multiply)
                        multiplier = -1

                    sensor.gasses[0].min_sensitivity = '{:g}'.format(base_number * multiplier - difference)
                    sensor.gasses[0].max_sensitivity = '{:g}'.format(base_number * multiplier + difference)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test = parser_utils.search_text(self.pdf, 'Response Time')
            if _t_test != '' and _t_test.lower() not in NULL_VALUES:
                sensor.gasses[0].t_test_type = _t_test.split()[0].lower()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test_response = parser_utils.search_next_column(self.pdf, 'Response Time', TABLE1)
            if _t_test_response != '' and _t_test_response.lower() not in NULL_VALUES:
                sensor.gasses[0].t_test_response = _t_test_response.split()[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].t_test_response_start = None
        sensor.gasses[0].t_test_response_end = None

        return sensor.to_dict()
