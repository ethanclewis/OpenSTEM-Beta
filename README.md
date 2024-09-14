## Archive
### *Arm_Gantry_Control.py*
Defines pins and functions to contorl forward (right) and backward (left) movement of Arm Gantry powered by a stepper motor.  
Distance and speed of arm movement are defined by *duration* and *delay*, respectively.   
**Not currently implemented in CofC system; stepper motor does not appear to have any power supply.** 


## Arduino to Pi
### *Arm_Gantry_Control.py*
Defines pins and functions to contorl forward (right) and backward (left) movement of Arm Gantry powered by a stepper motor.  
Distance and speed of arm movement are defined by *duration* and *delay*, respectively.   
**Not currently implemented in CofC system; stepper motor does not appear to have any power supply.** 


## DHT Sensors
### *DHTXX.py*
Reads temperature and humidity data from a DHT22 sensor (connected to Raspberry Pi GPIO pins 17, 18, and 24) every 60 seconds, and prints results to console.

## InfluxDB Testing
### *opennstem_fan.py*
Manually powers on system fans (connected to Raspberry Pi GPIO pin 19) for 10 seconds before turning them off.  
**Not currently implemented in CofC system; fans appear to constantly be on and controlled by independent power supply.** 


## Lights
### *automatic_light_schedule.py*
Automates the control of all system grow lights (connected to Raspberry Pi GPIO pin 26) based on current time of day. 
- Lights On from 8AM - 8PM  

Current time is checked at the top of every hour to verify correct light power state.  
Lights are turned off following script termination. 

### *manual_light_switch.py*
Provides a command-line interface to toggle all system grow lights (connected to Raspberry Pi GPIO pin 26).  
- "yes" = Lights On  
- "no" = Lights Off  

Lights remain in most recent power state after the script is terminated.

### *manual_individual_light_switch.py*
Framework to create command-line interface to individually toggle system grow lights (connected to Raspberry Pi GPIO pins 26, 13, 21, 27, 4, and 23).  
Lights are turned off following user prompted "Exit" or script termination.  
**Not currently implemented in CofC system; all lights controlled by GPIO pin 26.**

## Pump Control Tests
### *DHTXX.py*
Reads temperature and humidity data from a DHT22 sensor (connected to Raspberry Pi GPIO pins 17, 18, and 24) every 60 seconds, and prints results to console.

## Sensor Calibration
### *opennstem_fan.py*
Manually powers on system fans (connected to Raspberry Pi GPIO pin 19) for 10 seconds before turning them off.  
**Not currently implemented in CofC system; fans appear to constantly be on and controlled by independent power supply.** 