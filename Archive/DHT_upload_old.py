import Adafruit_DHT
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import certifi
from datetime import datetime
import os

# InfluxDB configuration
token = '8Kph_oFbnNqTvgUfERhTJveJpw1A08-HDsnhftEIYj8HRSUJSmKIdoPZdaIbNeIkG4likhDrOtr5FgyF5W03hg=='
org = "Ethan"
bucket = "Test"  # Use "bucket" instead of "database" in influxdb-client
url = "https://us-east-1-1.aws.cloud2.influxdata.com"

# Create InfluxDB client
client = InfluxDBClient(url=url, token=token, org=org, ssl_ca_cert=certifi.where())
write_api = client.write_api(write_options=SYNCHRONOUS)

# SETUP
sensor = Adafruit_DHT.DHT22     # Sensor type

# Define sensor pins for each DHT sensor
sensor_pins = {
    "DHT17": 17,
    "DHT18": 18,
    "DHT24": 24
}

# Log file path
log_file_path = '/home/openstem/Desktop/openstem_ethan/dht_sensor_log.txt'

# Function to log messages to a text file
def log_message(message):
    with open(log_file_path, 'a') as log_file:
        log_file.write(f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} - {message}\n")

# Function to get sensor readings
def get_sensor_readings(sensor, pin):
    humidity, temperature = Adafruit_DHT.read_retry(sensor, pin)
    
    if humidity is not None and temperature is not None:
        temperatureF = temperature * 9/5.0 + 32
        return temperature, temperatureF, humidity
    else:
        return None, None, None

# Function to upload DHT sensor data to InfluxDB
def upload_dht_data(data):
    for entry in data:
        point = (
            Point("dht_sensors")  # Measurement for DHT sensors
            .tag("sensor", entry["sensor"])  # Tag for sensor type
            .field("Temp_C", entry["Temp_C"])  # Temperature in Celsius
            .field("Temp_F", entry["Temp_F"])  # Temperature in Fahrenheit
            .field("Humidity", entry["Humidity"])  # Humidity level
        )
        write_api.write(bucket=bucket, org=org, record=point)
    
    # Log confirmation with the timestamp
    log_message(f"Data uploaded to InfluxDB")

# Main execution for getting readings and uploading data
dht_sensor_data = []  # Initialize list for storing sensor data

for sensor_name, pin in sensor_pins.items():
    # Get readings for each sensor
    temperature, temperatureF, humidity = get_sensor_readings(sensor, pin)

    if temperature is not None and humidity is not None:
        # Create sensor data dictionary and append to the list
        sensor_data = {
            "sensor": sensor_name,
            "Temp_C": round(temperature, 1),
            "Temp_F": round(temperatureF, 1),
            "Humidity": round(humidity, 1)
        }
        dht_sensor_data.append(sensor_data)
    else:
        log_message(f"{sensor_name}: Sensor failure. Check wiring.")

# Upload data to InfluxDB
upload_dht_data(dht_sensor_data)