#include <Wire.h>
#include <Ezo_i2c.h>

// Define the sensors
Ezo_board PH = Ezo_board(99, "PH");
Ezo_board EC = Ezo_board(100, "EC");
Ezo_board RTD = Ezo_board(102, "RTD");

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Arduino Ready");  // Signal Python that Arduino is ready
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'R') {  // 'R' for Read sensors
      poll_sensors();
    }
  }
  delay(100);
}

// Function to read and print sensor values
void poll_sensors() {
  float ph_value = read_ph();
  float ec_value = read_ec();
  float temp_value = read_temp();

  Serial.print("pH: "); Serial.println(ph_value);
  Serial.print("EC: "); Serial.println(ec_value);
  Serial.print("Temperature: "); Serial.println(temp_value);
}

// Function to read pH value
float read_ph() {
  char sensordata_buffer[32];
  PH.send_read_cmd();
  delay(1000);  // Wait for the sensor to process the command
  if (PH.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    return String(sensordata_buffer).toFloat();
  } else {
    Serial.println("Error reading pH sensor.");
    return -1;
  }
}

// Function to read EC value
float read_ec() {
  char sensordata_buffer[32];
  EC.send_read_cmd();
  delay(1000);  // Wait for the sensor to process the command
  if (EC.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    return String(sensordata_buffer).toFloat();
  } else {
    Serial.println("Error reading EC sensor.");
    return -1;
  }
}

// Function to read Temperature value
float read_temp() {
  char sensordata_buffer[32];
  RTD.send_read_cmd();
  delay(1000);  // Wait for the sensor to process the command
  if (RTD.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    return String(sensordata_buffer).toFloat();
  } else {
    Serial.println("Error reading Temperature sensor.");
    return -1;
  }
}