import RPi.GPIO as GPIO
import datetime

# SETUP
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)              # Reference GPIO pins by BCM numbers
relay_pin = 26                      # GPIO pin
GPIO.setup(relay_pin, GPIO.OUT)     # Set GPIO pin 26 to output pin

# Log file path for error messages
log_file_path = '/home/openstem/Desktop/openstem_ethan/light_log.txt'

# Function to log error messages to a text file
def log_error_message(message):
    with open(log_file_path, 'a') as log_file:
        log_file.write(f"{datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')} - {message}\n")

# LIGHT SCHEDULE
start_time = datetime.time(7, 59)  # 8AM (1 minute buffer to account for cron)
end_time = datetime.time(11, 59)   # 12PM (1 minute buffer to account for cron)

# Function to control the lights
def control_lights():
    now = datetime.datetime.now().time()
    if start_time <= now <= end_time:
        GPIO.output(relay_pin, GPIO.LOW)  # Lights ON
    else:
        GPIO.output(relay_pin, GPIO.HIGH)  # Lights OFF

# Run the function to control lights based on the time
try:
    control_lights()
except Exception as e:
    log_error_message(f"Error occurred: {e}")
#finally:
    #GPIO.cleanup()