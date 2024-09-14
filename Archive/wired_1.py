import serial
import time

# Initialize serial connection to Arduino
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)  # Wait for the serial connection to initialize

def read_arduino_data():
    if ser.in_waiting > 0:
        data = ser.readline().decode('utf-8').strip()
        return data
    return None

def parse_sensor_data(data):
    try:
        # Split the incoming data to extract the sensor values
        if "Temperature" in data:
            temp_value = float(data.split(":")[1].strip())
            return "temp", temp_value
        elif "pH" in data:
            ph_value = float(data.split(":")[1].strip())
            return "ph", ph_value
        elif "EC" in data:
            ec_value = float(data.split(":")[1].strip())
            return "ec", ec_value
    except ValueError:
        print("Error parsing data")
        return None, None

while True:
    sensor_data = read_arduino_data()
    if sensor_data:
        sensor_type, value = parse_sensor_data(sensor_data)
        if value is not None:
            print(value)  # Print only the numeric value
    time.sleep(1)