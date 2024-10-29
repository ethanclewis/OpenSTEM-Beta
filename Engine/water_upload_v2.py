import serial
from datetime import datetime
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import certifi
import time

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

# Initialize serial connection to Arduino
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)  # Wait for the serial connection to stabilize

# Function to log messages to a text file
def log_message(message):
    with open(log_file_path, 'a') as log_file:
        log_file.write(f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} - {message}\n")

# Function to fetch sensor data from Arduino
def fetch_sensor_data():
    ser.write(b'R')  # Send 'R' command to request sensor readings
    time.sleep(5)  # Wait for Arduino to respond

    data = {"pH": None, "EC": None, "Temperature": None}
    while ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        if "pH" in line:
            data["pH"] = float(line.split(":")[1].strip())
        elif "EC" in line:
            data["EC"] = float(line.split(":")[1].strip())
        elif "Temperature" in line:
            data["Temperature"] = float(line.split(":")[1].strip())
    return data

# Function to upload water sensor data to InfluxDB
def upload_water_data(data):
    if all(value is not None for value in data.values()):
        point = (
            Point("water_sensors")  # Measurement for water sensors
            .field("RTD", data["Temperature"])  # RTD value
            .field("pH", data["pH"])  # pH value
            .field("EC", data["EC"])  # Electrical conductivity
        )
        write_api.write(bucket=bucket, org=org, record=point)
        log_message("Data uploaded to InfluxDB")
    else:
        log_message("Incomplete data. Upload aborted.")

# Main execution
try:
    sensor_data = fetch_sensor_data()  # Fetch the latest sensor readings
    upload_water_data(sensor_data)  # Upload the data to InfluxDB
except Exception as e:
    log_message(f"Error: {str(e)}")