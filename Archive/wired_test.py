import serial
from datetime import datetime
import time

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
            print(water_sensor_data)  # Print the sensor data to the console

            # Reset sensor values after printing, so that it waits for new readings
            temp_value = None
            ph_value = None
            ec_value = None

    # Wait before checking the next set of data (to prevent excessive reading)
    time.sleep(1)