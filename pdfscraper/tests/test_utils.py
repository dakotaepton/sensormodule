import subprocess
import json
from collections import OrderedDict

from pdfscraper.models.sensor import Sensor
from pdfscraper.models import errors


def run_scraper(path):
    process = subprocess.run(['python3', '-m', 'pdfscraper.pdf_scraper', path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    print(process.stderr.decode('utf-8'))
    data = json.loads(process.stdout.decode('utf-8'))

    return data


def setUp(data, incorrect_values):
    incorrect_values[Sensor.GASSES] = []


def tearDown(data, incorrect_values):
    if data is not None and data.get(Sensor.GASSES) is None:
        print("Error: The parser did not complete.")
        return

    data_length = len(data) - 3 - 1

    for gas in data.get(Sensor.GASSES):
        data_length += len(gas)

    # Remove empty dicts and lists
    incorrect_values[Sensor.GASSES] = [gas for gas in incorrect_values.get(Sensor.GASSES) if len(gas) > 0]
    if len(incorrect_values.get(Sensor.GASSES)) == 0:
        incorrect_values.pop(Sensor.GASSES)

    incorrect_length = len(incorrect_values)
    if Sensor.GASSES in incorrect_values:
        incorrect_length -= 1
        for gas in incorrect_values.get(Sensor.GASSES):
            incorrect_length += len(gas)

    if data_length > 0:
        percent_correct = round(100 - (incorrect_length / data_length * 100), 2)
        print('{0}/{1} ({2}%)'.format(data_length - incorrect_length, data_length, str(percent_correct)))


def value_test(data, incorrect_values, key, value, gas_number):
    if key in data:
        key_value = data.get(key)
        if key_value != value:
            incorrect_values[key] = key_value
    # Is inside the Gas dict
    else:
        if len(incorrect_values[Sensor.GASSES]) < gas_number:
            incorrect_values[Sensor.GASSES].append({})

        if data.get(Sensor.GASSES) is not None and len(data.get(Sensor.GASSES)) >= gas_number:
            key_value = data.get(Sensor.GASSES)[gas_number - 1].get(key)
            if key_value != value:
                incorrect_values[Sensor.GASSES][gas_number - 1][key] = key_value
        else:
            incorrect_values[Sensor.GASSES][gas_number - 1][key] = None


def display_result(incorrect_values):
    print('Incorrect Values:')
    print(json.dumps(incorrect_values, indent=4, ensure_ascii=False))
