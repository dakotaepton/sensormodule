from datetime import datetime
from collections import OrderedDict


class Gas():
    def __init__(self):
        self.gas_name = None
        self.gas_empirical = None
        self.units = None
        self.resolution = None
        self.min_reading = None
        self.max_reading = None
        self.zero_current = None
        self.overgas_limit = None
        self.min_sensitivity = None
        self.max_sensitivity = None
        self.sensitivity_ratio = None
        self.sensitivity_concentration = None
        self.t_test_type = None
        self.t_test_response = None
        self.t_test_response_start = None
        self.t_test_response_end = None

    def to_dict(self):
        data = OrderedDict()
        data[Sensor.GAS_NAME] = self.gas_name
        data[Sensor.GAS_EMPIRICAL] = self.gas_empirical
        data[Sensor.UNITS] = self.units
        data[Sensor.RESOLUTION] = self.resolution
        data[Sensor.MIN_READING] = self.min_reading
        data[Sensor.MAX_READING] = self.max_reading
        data[Sensor.ZERO_CURRENT] = self.zero_current
        data[Sensor.OVERGAS_LIMIT] = self.overgas_limit
        data[Sensor.MIN_SENSITIVITY] = self.min_sensitivity
        data[Sensor.MAX_SENSITIVITY] = self.max_sensitivity
        data[Sensor.SENSITIVITY_RATIO] = self.sensitivity_ratio
        data[Sensor.SENSITIVITY_CONC] = self.sensitivity_concentration
        data[Sensor.T_TEST] = self.t_test_type
        data[Sensor.T_TEST_RESPONSE] = self.t_test_response
        data[Sensor.T_TEST_RESPONSE_START] = self.t_test_response_start
        data[Sensor.T_TEST_RESPONSE_END] = self.t_test_response_end

        return data


class Sensor():
    def __init__(self):
        self.manufacturer = None
        self.technology = None
        self.active = None
        self.part_number = None
        self.alias = None
        self.datasheet_rev = None

        self.diameter = None
        self.height = None
        self.pin_count = None
        self.pin_length = None
        self.weight = None

        self.min_temp = None
        self.max_temp = None
        self.min_pressure = None
        self.max_pressure = None
        self.min_humidity = None
        self.max_humidity = None

        self.min_load = None
        self.max_load = None
        self.bias = None
        self.max_signal_drift = None
        self.signal_drift_interval = None
        self.expected_life = None
        self.warranty = None

        self.gas_count = 1
        self.gasses = []
        self.add_gas_dict()

    def add_gas_dict(self):
        self.gasses.append(Gas())

    def to_dict(self):
        data = OrderedDict()
        data[Sensor.MANUFACTURER] = self.manufacturer
        data[Sensor.TECHNOLOGY] = self.technology
        data[Sensor.PART_NUMBER] = self.part_number
        data[Sensor.ALIAS] = self.alias
        data[Sensor.ACTIVE] = self.active
        data[Sensor.LAST_REV] = datetime.today().strftime('%Y-%m-%d')
        data[Sensor.DATASHEET_REV] = self.datasheet_rev
        data[Sensor.SERIES] = '4-series'

        data[Sensor.DIAMETER] = self.diameter
        data[Sensor.HEIGHT] = self.height
        data[Sensor.PIN_COUNT] = self.pin_count
        data[Sensor.PIN_LENGTH] = self.pin_length
        data[Sensor.WEIGHT] = self.weight

        data[Sensor.MIN_TEMP] = self.min_temp
        data[Sensor.MAX_TEMP] = self.max_temp

        # Convert 'Atmospheric +/- n%' to kPa
        try:
            if self.min_pressure is not None and 'atm' in self.min_pressure.lower():
                self.min_pressure = '{:g}'.format(101.325 - float(self.min_pressure.replace('%', '').split()[-1]) / 100 * 101.325)
            if self.max_pressure is not None and 'atm' in self.max_pressure.lower():
                self.max_pressure = '{:g}'.format(101.325 + float(self.max_pressure.replace('%', '').split()[-1]) / 100 * 101.325)
        except Exception:
            self.min_pressure = None
            self.max_pressure = None

        data[Sensor.MIN_PRESSURE] = self.min_pressure
        data[Sensor.MAX_PRESSURE] = self.max_pressure
        data[Sensor.MIN_HUMIDITY] = self.min_humidity
        data[Sensor.MAX_HUMIDITY] = self.max_humidity

        data[Sensor.MIN_LOAD] = self.min_load
        data[Sensor.MAX_LOAD] = self.max_load
        data[Sensor.BIAS] = self.bias
        data[Sensor.MAX_SIGNAL_DRIFT] = self.max_signal_drift
        data[Sensor.DRIFT_INTERVAL] = self.signal_drift_interval
        data[Sensor.EXPECTED_LIFE] = self.expected_life
        data[Sensor.GAS_COUNT] = self.gas_count
        data[Sensor.WARRANTY] = self.warranty

        data[Sensor.GASSES] = []
        for i in range(self.gas_count):
            data[Sensor.GASSES].append(self.gasses[i].to_dict())

        return data

    MANUFACTURER = 'BrandID'
    TECHNOLOGY = 'Technology'
    PART_NUMBER = 'PartNumber'
    ALIAS = 'Alias'
    ACTIVE = 'ActivePart'
    LAST_REV = 'LastReview'
    DATASHEET_REV = 'DatasheetReview'
    SERIES = 'Series'

    DIAMETER = 'Diameter'
    HEIGHT = 'Height'
    PIN_COUNT = 'PinCount'
    PIN_LENGTH = 'PinLength'
    WEIGHT = 'MaxWeight'

    MIN_TEMP = 'MinTemperature'
    MAX_TEMP = 'MaxTemperature'
    MIN_PRESSURE = 'MinPressure'
    MAX_PRESSURE = 'MaxPressure'
    MIN_HUMIDITY = 'MinHumidity'
    MAX_HUMIDITY = 'MaxHumidity'

    MIN_LOAD = 'MinLoad'
    MAX_LOAD = 'MaxLoad'
    BIAS = 'Bias'
    MAX_SIGNAL_DRIFT = 'MaxSignalDrift'
    DRIFT_INTERVAL = 'SignalDriftInterval'
    EXPECTED_LIFE = 'ExpectedLife'
    WARRANTY = 'Warranty'

    GAS_COUNT = 'GasCount'
    GASSES = 'Gasses'
    GAS_NAME = 'Fullname'
    GAS_EMPIRICAL = 'Name'
    UNITS = 'MeasurementUnits'
    RESOLUTION = 'Resolution'
    MIN_READING = 'MinReading'
    MAX_READING = 'MaxReading'
    ZERO_CURRENT = 'ZeroCurrent'
    OVERGAS_LIMIT = 'OvergasLimit'
    MIN_SENSITIVITY = 'MinSensitivity'
    MAX_SENSITIVITY = 'MaxSensitivity'
    SENSITIVITY_RATIO = 'SensitivityRatio'
    SENSITIVITY_CONC = 'SensitivityTestConcentration'
    T_TEST = 'TTestType'
    T_TEST_RESPONSE = 'TTestResponse'
    T_TEST_RESPONSE_START = 'ResponseStartLevel'
    T_TEST_RESPONSE_END = 'ResponseEndLevel'
