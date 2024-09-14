#include <Wire.h>
#include <Ezo_i2c.h>

// Define the pH sensor
Ezo_board PH = Ezo_board(99, "PH");

void setup() {
  Serial.begin(9600);      // Start serial communication
  Wire.begin();            // Start I2C communication
  Serial.println("pH Sensor Calibration");
  Serial.println("Use the following commands for calibration:");
  Serial.println("ph:cal,mid,7   -> Calibrate to pH 7");
  Serial.println("ph:cal,low,4   -> Calibrate to pH 4");
  Serial.println("ph:cal,high,10 -> Calibrate to pH 10");
  Serial.println("ph:cal,clear   -> Clear calibration");
}

void loop() {
  char sensordata_buffer[32];

  // Send a command to read pH value
  PH.send_read_cmd();
  delay(1000);  // Wait for the sensor to process the command

  // Receive and print the pH value
  if (PH.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    float ph_value = String(sensordata_buffer).toFloat();
    Serial.print("pH: ");
    Serial.println(ph_value);
  } else {
    Serial.println("Error reading pH sensor.");
  }

  // Check if any calibration command is entered
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    PH.send_cmd(command.c_str());
  }

  delay(1000);  // Wait 1 second before the next reading
}