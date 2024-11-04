import serial
from datetime import datetime
import time

# Initialize serial connection to Arduino
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)  # Wait for the serial connection to stabilize

# Log file path
log_file_path = '/home/openstem/Desktop/openstem_ethan/pump_control_log.txt'

# Function to log messages to a text file
def log_message(message):
    with open(log_file_path, 'a') as log_file:
        log_file.write(f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} - {message}\n")

# Function to trigger pump control on Arduino
def trigger_pump_control():
    try:
        ser.write(b'P')  # Send 'P' command to trigger pump control
        time.sleep(5)  # Wait for Arduino to process and respond

        # Read and log Arduino responses
        while ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            log_message(line)
            print(line)
        log_message("Pump control command executed successfully.")
        
    except Exception as e:
        log_message(f"Error: {str(e)}")

# Execute pump control (3 minute delay to allow for sensor reading and uploading)
time.sleep(180)
trigger_pump_control()