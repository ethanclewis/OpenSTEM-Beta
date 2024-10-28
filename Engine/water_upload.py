import serial
from datetime import datetime
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import certifi

# InfluxDB configuration
token = '8Kph_oFbnNqTvgUfERhTJveJpw1A08-HDsnhftEIYj8HRSUJSmKIdoPZdaIbNeIkG4likhDrOtr5FgyF5W03hg=='
org = "Ethan"
bucket = "Test"  # Use "bucket" instead of "database" in influxdb-client
url = "https://us-east-1-1.aws.cloud2.influxdata.com"

# Create InfluxDB client
client = InfluxDBClient(url=url, token=token, org=org, ssl_ca_cert=certifi.where())
write_api = client.write_api(write_options=SYNCHRONOUS)

# Log file path
log_file_path = '/home/openstem/Desktop/openstem_ethan/water_sensor_log.txt'

# Function to log messages to a text file
def log_message(message):
    with open(log_file_path, 'a') as log_file:
        log_file.write(f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} - {message}\n")

# Initialize serial connection to Arduino
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

# Initialize variables to store sensor data
temp_value = None
ph_value = None
ec_value = None

def read_arduino_data():
    if ser.in_waiting > 0:
        data = ser.readline().decode('utf-8').strip()
        return data
    return None

def parse_sensor_data(data):
    global temp_value, ph_value, ec_value
    try:
        # Split the incoming data to extract the sensor values
        if "Temperature" in data:
            temp_value = float(data.split(":")[1].strip())
        elif "pH" in data:
            ph_value = float(data.split(":")[1].strip())
        elif "EC" in data:
            ec_value = float(data.split(":")[1].strip())
    except ValueError:
        log_message("Error parsing data")

# Function to upload water sensor data to InfluxDB
def upload_water_data():
    if temp_value is not None and ph_value is not None and ec_value is not None:
        water_sensor_data = [
            {"RTD": temp_value, "pH": ph_value, "EC": ec_value}
        ]
        for entry in water_sensor_data:
            point = (
                Point("water_sensors")  # Measurement for water sensors
                .field("RTD", entry["RTD"])  # RTD value
                .field("pH", entry["pH"])  # pH value
                .field("EC", entry["EC"])  # Electrical conductivity
            )
            write_api.write(bucket=bucket, org=org, record=point)
        
        log_message("Data uploaded to InfluxDB")

# Read data from the Arduino until all sensor values are collected
while temp_value is None or ph_value is None or ec_value is None:
    sensor_data = read_arduino_data()
    if sensor_data:
        parse_sensor_data(sensor_data)

# Upload the data to InfluxDB
upload_water_data()