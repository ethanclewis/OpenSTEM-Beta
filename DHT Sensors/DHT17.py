import Adafruit_DHT
import time

# SETUP
sensor = Adafruit_DHT.DHT22     # Sensor type
sensor_pin = 17                 # GPIO pin


# SENSOR READING
while True:
    # Retry sensor reading if initial attempt fails
    humidity, temperature = Adafruit_DHT.read_retry(sensor, sensor_pin)

    # Valid sensor reading...
    if humidity is not None and temperature is not None:
        # Create Fahrenheit value
        temperatureF = temperature * 9/5.0 + 32
        # Display temperature (C and F) and humidity values
        print("Temp={0:0.1f}ºC, Temp={1:0.1f}ºF, Humidity={2:0.1f}%".format(temperature, temperatureF, humidity))

    # Invalid sensor reading...
    else:
        print("Sensor failure. Check wiring.")

    # Take sensor reading every 60 seconds
    time.sleep(60)