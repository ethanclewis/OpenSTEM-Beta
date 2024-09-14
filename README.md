# Arduino to Pi Testing
### *rest.ino*
Blank sketch to "rest" the Arduino (do nothing); useful during sketch testing. 
### *wired_to_pi_TEST.ino*
Continuosly polls and processes RTD, pH, and EC sensor data in batches retrievable by the Raspberry Pi.
### *wired_to_ino_TEST.py*
Works in tandem with *wired_to_pi_TEST.ino* to retreive readings from RTD, pH, and EC sensors, and prints readings to the console every 1 second.  
### *wired_to_ino_TEST2.py*
Works in tandem with *wired_to_pi_TEST.ino* to retreive readings from RTD, pH, and EC sensors, and uploads data to the respective InfluxDB bucket/ measurement every 10 seconds **(influxdb_client package)**.  


# DHT Sensor Testing
### *DHT_all.py*
Reads temperature and humidity data from **all** DHT22 sensors every 60 seconds, and prints results to console.
### *DHTXX.py*
Reads temperature and humidity data from an **individual** DHT22 sensor every 60 seconds, and prints results to console.


# Engine
All scripts collaborate to automate system data uploading and environment regulation and are desinged to be continuously run by the Raspberry Pi's command-line job scheduler: cron.
### *wired_to_pi.ino*
Continuosly polls and processes RTD, pH, and EC sensor data in batches retrievable by the Raspberry Pi, as well as checks and regulates system pH and EC levels every 60 sec.   
**No cron scheduling required**
### *DHT_upload.py*
Retrieves readings from all three DHT sensors, and uploads data to the respective InfluxDB bucket/ measurement **(influxdb_client package)**.  
Successful data uploads and errors are logged in a .txt file.  
**Execute every 60 seconds with cron (\* \* \* \* \*)**
### *water_upload.py*
Works in tandem with *wired_to_pi.ino* to retreive readings from RTD, pH, and EC sensors, and uploads data to the respective InfluxDB bucket/ measurement **(influxdb_client package)**.  
Successful data uploads and errors are logged in a .txt file.  
**Execute every 60 seconds with cron (\* \* \* \* \*)**


# InfluxDB Testing
### *DHT_upload_TEST (InfluxDB3).py*
Retrieves readings from all three DHT sensors, and uploads data to the respective InfluxDB bucket/ measurement **(influxdb_client_3 package)**.
### *InfluxDB_TEST.ipynb*
Playground environment to understand and visualize InfluxDB uploading mechanics using simple test data.  
**(influxdb_client package)**
### *InfluxDB3_TEST.ipynb*
Playground environment to understand and visualize InfluxDB3 uploading mechanics using simple test data.  
**(influxdb_client_3 package)**


# Lights
### *automated_lights_seeds.py*
Automates the control of all system grow lights based on current time of day during seedling/ germination period.
- Lights On from 8AM - 12PM  

**Execute every 1 hour with cron (0 \* \* \* \*)**
### *automated_lights.py*
Automates the control of all system grow lights based on current time of day during adulthood.
- Lights On from 8AM - 8PM  

**Execute every 1 hour with cron (0 \* \* \* \*)**
### *manual_light_switch.py*
Provides a command-line interface to toggle all system grow lights.  
Lights remain in most recent power state after the script is terminated.


# Pump Control Testing
- **PMP1:** pH Up
- **PMP2:** pH Down
- **PMP3:** Nutrients (EC Up)  

### *PMP_Test.ino*
Tests **all** pump behavior based on sensor reading thresholds. User can update threshold values via serial monitor commands. 
### *PMPX_Test.ino*
Tests **individual** pump behavior based on sensor reading threshold. User can update threshold value via serial monitor commands. 


# Sensor Calibration
Calibrates EC, pH, and RTD sensors via Arduino serial monitor; calibration commands/ instructions stored in sketches and printed to serial monitor.