# Arduino to Pi Testing
### *rest.ino*
### *wired_to_pi_TEST.ino*
### *wired_to_ino_TEST.py*
### *wired_to_ino_TEST2.py*


# DHT Sensors
## InfluxDB Upload Testing
### *DHT_upload_TEST (InfluxDB3).py*
### *DHT_upload_TEST.py*
## Read Testing
### *DHT_all.py*
### *DHTXX.py*


# Engine
All scripts collaborate to automate system data uploading and environment regulation and are desinged to be continuously run by the Raspberry Pi's command-line job scheduler: cron.
### *wired_to_pi.ino*
Continuosly polls and processes RTD, pH, and EC sensor data in batches retrievable by the Raspberry Pi, as well as checks and regulates system pH and EC levels every 60 sec.  
**No cron scheduling required**
### *DHT_upload.py*
Retrieves readings from all three DHT sensors, and uploads data to the respective InfluxDB bucket/ measurement.  
**Execute every 60 seconds with cron (* * * * *)**
### *water_upload.py*
Works in tandem with *wired_to_pi.ino* to retreive readings from RTD, pH, and EC sensors, and uploads data to the respective InfluxDB bucket/ measurement.   
**Execute every 60 seconds with cron (* * * * *)**

# InfluxDB Testing
### *InfluxDB_TEST.ipynb*
### *InfluxDB3_TEST.ipynb*


# Lights
### *automated_lights_seeds.py*
### *automated_lights.py*
### *manual_light_switch.py*


# Pump Control Testing
### *PMP_Test.ino*
### *PMPX_Test.ino*


# Sensor Calibration