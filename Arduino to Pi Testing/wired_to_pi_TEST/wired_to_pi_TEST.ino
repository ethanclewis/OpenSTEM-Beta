#include <Wire.h>
#include <Ezo_i2c.h>

// Define the sensors
Ezo_board PH = Ezo_board(99, "PH");
Ezo_board EC = Ezo_board(100, "EC");
Ezo_board RTD = Ezo_board(102, "RTD");

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  char sensordata_buffer[32];

  // Read and print pH value
  PH.send_read_cmd();
  delay(1000); // Wait for the sensor to process the command
  if (PH.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    float ph_value = String(sensordata_buffer).toFloat();
    Serial.print("pH: ");
    Serial.println(ph_value);
  } else {
    Serial.println("Error reading pH sensor.");
  }

  // Read and print EC value
  EC.send_read_cmd();
  delay(1000); // Wait for the sensor to process the command
  if (EC.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    float ec_value = String(sensordata_buffer).toFloat();
    Serial.print("EC: ");
    Serial.println(ec_value);
  } else {
    Serial.println("Error reading EC sensor.");
  }

  // Read and print Temperature value
  RTD.send_read_cmd();
  delay(1000); // Wait for the sensor to process the command
  if (RTD.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    float temp_value = String(sensordata_buffer).toFloat();
    Serial.print("Temperature: ");
    Serial.println(temp_value);
  } else {
    Serial.println("Error reading Temperature sensor.");
  }

  delay(1000); // Wait 5 seconds before next reading
}