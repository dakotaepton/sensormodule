import pdfquery

from pdfscraper.models.sensor import Sensor
from pdfscraper.models.manufacturer import Manufacturer
from pdfscraper.parsers import parser_utils
from pdfscraper.parsers.parser_utils import Justification

# Widths of each table column in points (1 inch = 72 pts, measured using Adobe PDF)
TABLE = (153.36, 165.2)

# Strings in datasheets representing an empty specification
NULL_VALUES = ['not required']

U_PLUS_MINUS = '\u00b1'
U_LESS_THAN_EQUAL = '\u2264'
U_MICRO_SMALL = '\u00b5'
U_MICRO = '\u03bc'


class CityTechParserFormat1:
    def __init__(self, pdf):
        """Initialises the parser of CityTech datasheets with the table like
        format.

        Args:
            pdf (PDFQuery): A query wrapper around a supplied PDF for data to
                be extracted from.
        """
        self.pdf = pdf

    def parse(self):
        """Extracts each sensor specification from a CityTech datasheet.

        Returns:
            dict: A dictionary filled in the found values. Any values that
            could not be found will be defaulted to the None type.
        """
        sensor = Sensor()

        dual_gas_sensor = False
        _dual_gas_sensor = parser_utils.search_text(self.pdf, 'Dual Gas Sensor')
        if _dual_gas_sensor != '':
            sensor.gas_count = 2
            sensor.add_gas_dict()

        sensor.manufacturer = Manufacturer.CITYTECH

        try:
            _technology = parser_utils.search_next_column(self.pdf, 'Operating Principle', TABLE, justification=Justification.RIGHT, page_num=1)
            if _technology == '':
                _technology = parser_utils.search_next_column(self.pdf, 'Technology', TABLE, justification=Justification.RIGHT, page_num=1)
            if 'electrochemical' in _technology.lower():
                sensor.technology = 'EC'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _part_number = parser_utils.search_text(self.pdf, 'Part Number:', page_num=1)
            sensor.part_number = _part_number.split(': ')[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _alias = parser_utils.search_text(self.pdf, 'CiTiceL', page_num=1)
            sensor.alias = ' '.join(_alias.split()[:-1])
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.active = True

        sensor.datasheet_rev = None

        try:
            _pin_count = parser_utils.search_next_column(self.pdf, 'Operating Principle', TABLE, justification=Justification.RIGHT, page_num=1)
            if _pin_count != '' and _pin_count[0].isdigit():
                sensor.pin_count = _pin_count[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _weight = parser_utils.search_next_column(self.pdf, 'Weight', TABLE, justification=Justification.RIGHT, page_num=1)
            if U_PLUS_MINUS not in _weight:
                sensor.weight = _weight.replace('Approx. ', '').split()[0].replace('g', '')
            else:
                _weight = _weight.split()
                base = float(_weight[0])
                difference = float(_weight[2])
                sensor.weight = '{:g}'.format(base + difference)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _temp = parser_utils.search_next_column(self.pdf, 'Temperature', TABLE, justification=Justification.RIGHT, page_num=1)
            if _temp == '':
                _temp = parser_utils.search_next_column(self.pdf, 'Temperature', TABLE, justification=Justification.RIGHT, search_n_rows=2, page_num=1)
            _temp = ' '.join(_temp.split()[:3])
            _temp = _temp.replace('°C', '').replace('+', '')
            sensor.min_temp = _temp.split(' to ')[0]
            sensor.max_temp = _temp.split(' to ')[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _pressure = parser_utils.search_next_column(self.pdf, 'Pressure', TABLE, justification=Justification.RIGHT, page_num=1)
            if 'atm' in _pressure.lower():
                _pressure = _pressure.split()[-1].replace('%', '')
                sensor.min_pressure = 'Atmospheric - ' + _pressure + '%'
                sensor.max_pressure = 'Atmospheric + ' + _pressure + '%'
            elif 'mbar' in _pressure:
                _pressure = _pressure.split()
                sensor.min_pressure = str(int(_pressure[0]) // 10)
                sensor.max_pressure = str(int(_pressure[2]) // 10)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _humidity = parser_utils.search_next_column(self.pdf, 'Humidity', TABLE, justification=Justification.RIGHT, page_num=1)
            if _humidity == '':
                _humidity = parser_utils.search_next_column(self.pdf, 'Humidity', TABLE, justification=Justification.RIGHT, search_n_rows=2, page_num=1)
            _humidity = _humidity.lower().replace('rh', '').replace(' -', '% to').replace(' to ', '').split('%')
            sensor.min_humidity = _humidity[0].strip()
            sensor.max_humidity = _humidity[1].strip()
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _load = parser_utils.search_next_column(self.pdf, 'Load Resistor', TABLE, justification=Justification.RIGHT, page_num=1)
            if _load != '':
                sensor.min_load = _load.split()[0]
                sensor.max_load = _load.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _bias = parser_utils.search_next_column(self.pdf, 'Bias Voltage', TABLE, justification=Justification.RIGHT, page_num=1)
            if _bias.lower() not in NULL_VALUES:
                sensor.bias = _bias.split()[0].replace('+', '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _drift = parser_utils.search_next_column(self.pdf, 'Drift', TABLE, justification=Justification.RIGHT, page_num=1)
            if _drift == '':
                _drift = parser_utils.search_next_column(self.pdf, 'Drift', TABLE, justification=Justification.RIGHT, page_num=2)
            _drift = _drift.split('%')[0].replace('<', '').strip()
            if _drift != '':
                sensor.max_signal_drift = _drift
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _expected_life = parser_utils.search_next_column(self.pdf, 'Operating Life', TABLE, justification=Justification.RIGHT, page_num=1)
            if _expected_life == '':
                _expected_life = parser_utils.search_next_column(self.pdf, 'Operating Life', TABLE, justification=Justification.RIGHT, page_num=2)
            _number = _expected_life.split()[0].lower()

            if _number == 'one':
                _number = '1'
            elif _number == 'two':
                _number = '2'

            if 'year' in _expected_life:
                sensor.expected_life = str(int(_number) * 12)
            else:
                sensor.expected_life = _number
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _drift_interval = parser_utils.search_next_column(self.pdf, 'Drift', TABLE, justification=Justification.RIGHT, page_num=1)
            if _drift_interval == '':
                _drift_interval = parser_utils.search_next_column(self.pdf, 'Drift', TABLE, justification=Justification.RIGHT, page_num=2)
            if 'operating life' in _drift_interval:
                sensor.signal_drift_interval = sensor.expected_life
            elif _drift_interval != '':
                _drift_interval = _drift_interval.split()[-1]
                if _drift_interval in ['annum', 'year']:
                    sensor.signal_drift_interval = '12'
                elif _drift_interval == 'month':
                    sensor.signal_drift_interval = '1'
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _warranty = parser_utils.search_next_column(self.pdf, 'Warranty', TABLE, justification=Justification.RIGHT, page_num=1)
            if _warranty == '':
                _warranty = parser_utils.search_next_column(self.pdf, 'Warranty', TABLE, justification=Justification.RIGHT, page_num=2)
            if _warranty != '':
                sensor.warranty = _warranty.split()[0]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _gas_name = parser_utils.search_text(self.pdf, 'Gas Sensor', page_num=1)
            if sensor.gas_count == 1:
                if 'Industrial Safety' in _gas_name:
                    sensor.gasses[0].gas_name = ' '.join(_gas_name.split()[0:-6])
                else:
                    sensor.gasses[0].gas_name = ' '.join(_gas_name.split()[0:-3])
            elif sensor.gas_count == 2:
                _gas_name = _gas_name.split('(')[0].strip()
                sensor.gasses[0].gas_name = _gas_name.split(' / ')[0]
                sensor.gasses[1].gas_name = _gas_name.split(' / ')[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _gas_empirical = parser_utils.search_text(self.pdf, 'Gas Sensor', page_num=1)
            if sensor.gas_count == 1:
                if 'Industrial Safety' in _gas_name:
                    sensor.gasses[0].gas_empirical = _gas_empirical.split()[-6].replace('(', '').replace(')', '')
                else:
                    sensor.gasses[0].gas_empirical = _gas_empirical.split()[-3].replace('(', '').replace(')', '')
            elif sensor.gas_count == 2:
                _gas_empirical = _gas_empirical.split('(')[1].split(')')[0]
                sensor.gasses[0].gas_empirical = _gas_empirical.split(' / ')[0]
                sensor.gasses[1].gas_empirical = _gas_empirical.split(' / ')[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _units = parser_utils.search_next_column(self.pdf, 'Measurement Range', TABLE, justification=Justification.RIGHT, page_num=1)
            if _units == '':
                _units = parser_utils.search_next_column(self.pdf, 'Detection Range', TABLE, justification=Justification.RIGHT, page_num=1)
            if sensor.gas_count == 1:
                _units = _units.replace(' - ', '-').replace(' to ', '-').split()[1]
                if 'vol' in _units:
                    sensor.gasses[0].units = '%Vol'
                else:
                    sensor.gasses[0].units = _units
            elif sensor.gas_count == 2:
                sensor.gasses[0].units = _units.split()[3]
                sensor.gasses[1].units = _units.split()[-1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            if sensor.gas_count == 1:
                _resolution = parser_utils.search_next_column(self.pdf, 'Resolution', TABLE, justification=Justification.RIGHT, page_num=1)
                if _resolution != '':
                    if 'Dependent on electronics' in _resolution:
                        _resolution = parser_utils.search_next_column(self.pdf, 'Resolution', TABLE, justification=Justification.RIGHT, search_n_rows=2, page_num=1)
                        sensor.gasses[0].resolution = _resolution.split('(')[1].split()[0]
                    else:
                        sensor.gasses[0].resolution = _resolution.replace('< ', '<').split()[0].replace('<', '')
            elif sensor.gas_count == 2:
                _resolution = parser_utils.search_next_column(self.pdf, 'Resolution', TABLE, justification=Justification.RIGHT, search_n_rows=2, page_num=1)
                sensor.gasses[0].resolution = '{:g}'.format(float(_resolution.split()[2]))
                sensor.gasses[1].resolution = '{:g}'.format(float(_resolution.split()[-2]))
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            if sensor.gas_count == 1:
                _reading = parser_utils.search_next_column(self.pdf, 'Measurement Range', TABLE, justification=Justification.RIGHT, page_num=1)
                if _reading == '':
                    _reading = parser_utils.search_next_column(self.pdf, 'Detection Range', TABLE, justification=Justification.RIGHT, page_num=1)
                _reading = _reading.replace(' - ', '-').replace(' to ', '-').split()[0]
                sensor.gasses[0].min_reading = _reading.split('-')[0].replace('%', '')
                sensor.gasses[0].max_reading = _reading.split('-')[1].replace('%', '')
            elif sensor.gas_count == 2:
                _reading = parser_utils.search_next_column(self.pdf, 'Measurement Range', TABLE, justification=Justification.RIGHT, search_n_rows=2, page_num=1)
                sensor.gasses[0].min_reading = _reading.split()[2].split('-')[0]
                sensor.gasses[0].max_reading = _reading.split()[2].split('-')[1]
                sensor.gasses[1].min_reading = _reading.split()[-2].split('-')[0]
                sensor.gasses[1].max_reading = _reading.split()[-2].split('-')[1]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].zero_current = None

        try:
            if sensor.gas_count == 1:
                _overgas = parser_utils.search_next_column(self.pdf, 'Overload', TABLE, justification=Justification.RIGHT, page_num=1)
                if _overgas != '':
                    sensor.gasses[0].overgas_limit = _overgas.split()[0].replace('%', '')
            elif sensor.gas_count == 2:
                _overgas = parser_utils.search_next_column(self.pdf, 'Overload', TABLE, justification=Justification.RIGHT, search_n_rows=2, page_num=1)
                sensor.gasses[0].overgas_limit = _overgas.split()[2]
                sensor.gasses[1].overgas_limit = _overgas.split()[-2]
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            if sensor.gas_count == 1:
                if sensor.gasses[0].units == 'ppm':
                    _sensitivity_ratio = parser_utils.search_next_column(self.pdf, 'Sensitivity', TABLE, justification=Justification.RIGHT, page_num=1)
                    if _sensitivity_ratio == '':
                        _sensitivity_ratio = parser_utils.search_next_column(self.pdf, 'Sensitivity*', TABLE, justification=Justification.RIGHT, page_num=1)
                    sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio.split()[-1].replace(U_MICRO_SMALL, U_MICRO)
                elif sensor.gasses[0].units == '%Vol':
                    _sensitivity_ratio = parser_utils.search_next_column(self.pdf, 'Output Signal', TABLE, justification=Justification.RIGHT, page_num=1)
                    if _sensitivity_ratio != '':
                        sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio.split()[-3] + '/ppm'
            elif sensor.gas_count == 2:
                _sensitivity_ratio = parser_utils.search_next_column(self.pdf, 'Sensitivity', TABLE, justification=Justification.RIGHT, search_n_rows=2, page_num=1)
                sensor.gasses[0].sensitivity_ratio = _sensitivity_ratio.split()[5].replace(U_MICRO_SMALL, U_MICRO)
                sensor.gasses[1].sensitivity_ratio = _sensitivity_ratio.split()[-1].replace(U_MICRO_SMALL, U_MICRO)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].sensitivity_concentration = None

        try:
            if sensor.gas_count == 1:
                if sensor.gasses[0].units == 'ppm':
                    _sensitivity = parser_utils.search_next_column(self.pdf, 'Sensitivity', TABLE, justification=Justification.RIGHT, page_num=1)
                    if _sensitivity == '':
                        _sensitivity = parser_utils.search_next_column(self.pdf, 'Sensitivity*', TABLE, justification=Justification.RIGHT, page_num=1)
                    base = float(_sensitivity.split()[0])
                    difference = float(_sensitivity.split()[2])
                    sensor.gasses[0].min_sensitivity = '{:g}'.format(base - difference)
                    sensor.gasses[0].max_sensitivity = '{:g}'.format(base + difference)
                elif sensor.gasses[0].units == '%Vol':
                    _sensitivity_ratio = parser_utils.search_next_column(self.pdf, 'Output Signal', TABLE, justification=Justification.RIGHT, page_num=1)
                    sensor.gasses[0].min_sensitivity = '{:g}'.format(float(_sensitivity_ratio.split()[0]))
                    sensor.gasses[0].max_sensitivity = '{:g}'.format(float(_sensitivity_ratio.split()[2]))
            elif sensor.gas_count == 2:
                _sensitivity = parser_utils.search_next_column(self.pdf, 'Sensitivity', TABLE, justification=Justification.RIGHT, search_n_rows=2, page_num=1)
                base = float(_sensitivity.split()[2])
                difference = float(_sensitivity.split()[4])
                sensor.gasses[0].min_sensitivity = '{:g}'.format(base - difference)
                sensor.gasses[0].max_sensitivity = '{:g}'.format(base + difference)
                base = float(_sensitivity.split()[-4])
                difference = float(_sensitivity.split()[-2])
                sensor.gasses[1].min_sensitivity = '{:g}'.format(base - difference)
                sensor.gasses[1].max_sensitivity = '{:g}'.format(base + difference)
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            _t_test = parser_utils.search_text(self.pdf, 'Response Time', page_num=1)
            if sensor.gasses[0].units == 'ppm':
                sensor.gasses[0].t_test_type = _t_test.split(' (')[1].split(')')[0].lower()
                if sensor.gas_count == 2:
                    sensor.gasses[1].t_test_type = sensor.gasses[0].t_test_type
            else:
                sensor.gasses[0].t_test_type = _t_test.split()[0].lower()
                if sensor.gas_count == 2:
                    sensor.gasses[1].t_test_type = sensor.gasses[0].t_test_type
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        try:
            if sensor.gas_count == 1:
                _t_test_response = parser_utils.search_next_column(self.pdf, 'Response Time', TABLE, justification=Justification.RIGHT, page_num=1)
                if 'at' in _t_test_response:
                    sensor.gasses[0].t_test_response = _t_test_response.split()[-4].replace('<', '').replace(U_LESS_THAN_EQUAL, '')
                else:
                    sensor.gasses[0].t_test_response = _t_test_response.split()[-2].replace('<', '').replace(U_LESS_THAN_EQUAL, '')
            elif sensor.gas_count == 2:
                _t_test_response = parser_utils.search_next_column(self.pdf, 'Response Time', TABLE, justification=Justification.RIGHT, search_n_rows=2, page_num=1)
                sensor.gasses[0].t_test_response = _t_test_response.split()[2].replace('<', '').replace(U_LESS_THAN_EQUAL, '')
                sensor.gasses[1].t_test_response = _t_test_response.split()[-4].replace('<', '').replace(U_LESS_THAN_EQUAL, '')
        except (IndexError, ValueError, TypeError, AttributeError) as e:
            parser_utils.log_error()

        sensor.gasses[0].t_test_response_start = None
        sensor.gasses[0].t_test_response_end = None

        return sensor.to_dict()
