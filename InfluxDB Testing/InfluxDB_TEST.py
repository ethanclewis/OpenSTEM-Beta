import time
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import certifi
import pandas as pd


# InfluxDB configuration
token = '8Kph_oFbnNqTvgUfERhTJveJpw1A08-HDsnhftEIYj8HRSUJSmKIdoPZdaIbNeIkG4likhDrOtr5FgyF5W03hg=='
org = "Ethan"
bucket = "Test"  # Use "bucket" instead of "database" in influxdb-client
url = "https://us-east-1-1.aws.cloud2.influxdata.com"

# Create InfluxDB client
client = InfluxDBClient(url=url, token=token, org=org, ssl_ca_cert=certifi.where())
write_api = client.write_api(write_options=SYNCHRONOUS)


# Example sensor data (hardcoded)
dht_sensor_data = [
    {"Zone": "Zone 1", "Temp_C": 20.5, "Temp_F": 68.9, "Humidity": 55.0},
    {"Zone": "Zone 2", "Temp_C": 21.0, "Temp_F": 69.8, "Humidity": 60.0},
    {"Zone": "Zone 3", "Temp_C": 19.8, "Temp_F": 67.6, "Humidity": 58.0},
]

water_sensor_data = [
    {"sensor": "water_sensor", "RTD": 0.98, "pH": 6.5, "EC": 1.1},
    {"sensor": "water_sensor", "RTD": 0.95, "pH": 6.6, "EC": 1.2},
    {"sensor": "water_sensor", "RTD": 0.99, "pH": 6.4, "EC": 1.3},
]


# Function to upload DHT sensor data to InfluxDB
def upload_dht_data(data):
    for entry in data:
        point = (
            Point("dht_sensors")  # Measurement for DHT sensors
            .tag("Zone", entry["Zone"])  # Tag for sensor type
            .field("Temp_C", entry["Temp_C"])  # Temperature in Celsius
            .field("Temp_F", entry["Temp_F"])  # Temperature in Fahrenheit
            .field("Humidity", entry["Humidity"])  # Humidity level
        )
        write_api.write(bucket=bucket, org=org, record=point)
        print(f"Uploaded DHT sensor data from {entry['Zone']}: {entry}")
        time.sleep(1)  # Optional: Delay to simulate real-time data collection

# Function to upload water sensor data to InfluxDB
def upload_water_data(data):
    for entry in data:
        point = (
            Point("water_sensors")  # Measurement for water sensors
            .field("RTD", entry["RTD"])  # RTD value
            .field("pH", entry["pH"])  # pH value
            .field("EC", entry["EC"])  # Electrical conductivity
        )
        write_api.write(bucket=bucket, org=org, record=point)
        print(f"Uploaded water sensor data: {entry}")
        time.sleep(1)  # Optional: Delay to simulate real-time data collection


# Upload data for DHT sensors
upload_dht_data(dht_sensor_data)
# Upload data for water sensors
upload_water_data(water_sensor_data)