import Adafruit_DHT
import time
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import certifi
from datetime import datetime

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
    
    # Print confirmation with top of the minute timestamp
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    print(f"Data uploaded to InfluxDB at {timestamp} for the top of the minute.")

# SENSOR READING LOOP
while True:
    # Wait until the top of the next minute
    now = datetime.now()
    sleep_time = 60 - now.second  # Calculate sleep time to align with top of the minute
    time.sleep(sleep_time)

    dht_sensor_data = []  # Initialize list for storing sensor data

    for sensor_name, pin in sensor_pins.items():
        # Get readings for each sensor
        temperature, temperatureF, humidity = get_sensor_readings(sensor, pin)

        if temperature is not None and humidity is not None:
            # Optional: Print results to console
            print(f"{sensor_name}: Temp={temperature:0.1f}ºC, Temp={temperatureF:0.1f}ºF, Humidity={humidity:0.1f}%")
            
            # Create sensor data dictionary and append to the list
            sensor_data = {
                "sensor": sensor_name,
                "Temp_C": round(temperature, 1),
                "Temp_F": round(temperatureF, 1),
                "Humidity": round(humidity, 1)
            }
            dht_sensor_data.append(sensor_data)
        else:
            print(f"{sensor_name}: Sensor failure. Check wiring.")

    # Upload data to InfluxDB
    upload_dht_data(dht_sensor_data)