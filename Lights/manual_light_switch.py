import RPi.GPIO as GPIO
import time

# SETUP
GPIO.setmode(GPIO.BCM)              # Reference GPIO pins by BCM numbers
relay_pin = 26                      # GPIO pin 
GPIO.setup(relay_pin, GPIO.OUT)     # Set GPIO pin 26 to output pin


# LIGHT SWITH
# Continuously run script
try:
    while True:
        # User prompt
        choice = input("Turn on the lights? (yes/no): ")
        # Turn on lights     
        if choice == "yes":
            GPIO.output(relay_pin, GPIO.LOW)
            print("Lights ON")
        # Turn off lights    
        else:
            GPIO.output(relay_pin, GPIO.HIGH)
            print("Lights OFF")

# Terminate script with 'Ctrl+C'                
except KeyboardInterrupt:
    pass

# Uncomment to force lights off following script termination. Otherwise, remain in most recent state.  
# GPIO.cleanup()
                    
