import Adafruit_DHT
import time

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

# SENSOR READING LOOP
while True:
    dht_sensor_data = []  # Initialize list for storing sensor data

    for sensor_name, pin in sensor_pins.items():
        # Get readings for each sensor
        temperature, temperatureF, humidity = get_sensor_readings(sensor, pin)

        if temperature is not None and humidity is not None:
            # Print results to console
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

    # Print the final object for verification (optional)
    print(dht_sensor_data)

    # Take sensor reading every 60 seconds
    time.sleep(60)