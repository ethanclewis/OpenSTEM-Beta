import serial
from datetime import datetime
import time
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
        log_error("Error parsing data")

def log_error(message):
    with open("/home/openstem/Desktop/openstem_ethan/water_sensor_errors.log", 'a') as f:
        f.write(f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} - {message}\n")

# Function to upload water sensor data to InfluxDB
def upload_water_data(data):
    for entry in data:
        point = (
            Point("water_sensors")  # Measurement for water sensors
            .tag("sensor", entry["sensor"])  # Tag for sensor type
            .field("RTD", entry["RTD"])  # RTD value
            .field("pH", entry["pH"])  # pH value
            .field("EC", entry["EC"])  # Electrical conductivity
        )
        write_api.write(bucket=bucket, org=org, record=point)
        print(f"Uploaded water sensor data from {entry['sensor']}: {entry}")

# Main execution loop
while True:
    sensor_data = read_arduino_data()
    if sensor_data:
        parse_sensor_data(sensor_data)

        # Check if all sensor values are present
        if temp_value is not None and ph_value is not None and ec_value is not None:
            water_sensor_data = [
                {"sensor": "water_sensor", "RTD": temp_value, "pH": ph_value, "EC": ec_value}
            ]
            upload_water_data(water_sensor_data)  # Upload the sensor data to InfluxDB

            # Reset sensor values after uploading, so that it waits for new readings
            temp_value = None
            ph_value = None
            ec_value = None

    # Wait before checking the next set of data (to prevent excessive reading)
    time.sleep(10)