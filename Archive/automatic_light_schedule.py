import RPi.GPIO as GPIO
import datetime
import time

# SETUP
GPIO.setmode(GPIO.BCM)              # Reference GPIO pins by BCM numbers
relay_pin = 26                      # GPIO pin 
GPIO.setup(relay_pin, GPIO.OUT)     # Set GPIO pin 26 to output pin


# LIGHT SCHEDULE
start_time = datetime.time(8, 0)  # 8AM
end_time = datetime.time(20, 0)   # 8PM

# Function to physically turn on and off lights
def update_lights():
    now = datetime.datetime.now().time()
    if start_time <= now <= end_time:
        GPIO.output(relay_pin, GPIO.LOW)        # Lights on
    else:
        GPIO.output(relay_pin, GPIO.HIGH)       # Lights off


# AUTOMATED LIGHT SWITCH
# Continuously run script
try:
    while True:
        # Check and update light status
        update_lights()

        # Repeat check and update of light status at the start of every hour
        next_hour = (datetime.datetime.now() + datetime.timedelta(hours=1)).replace(minute=0, second=0, microsecond=0)
        sleep_time = (next_hour - datetime.datetime.now()).total_seconds()
        time.sleep(sleep_time)

# Terminate script with 'Ctrl+C'
except KeyboardInterrupt:
    pass

# Turn off lights following script termination
# If lights are off during the day, we know the script terminated
GPIO.cleanup()