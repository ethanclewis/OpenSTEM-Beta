#include <Wire.h>
#include <Ezo_i2c.h>

// Define the EC sensor
Ezo_board EC = Ezo_board(100, "EC");

void setup() {
  Serial.begin(9600);      // Start serial communication
  Wire.begin();            // Start I2C communication
  Serial.println("EC Sensor Calibration");
  Serial.println("Use the following commands for calibration:");
  Serial.println("ec:cal,dry           -> Calibrate a dry EC probe");
  Serial.println("ec:k,[n]             -> Switch K values (standard K values: 0.1, 1, 10)");
  Serial.println("ec:cal,clear         -> Clear calibration");

  // Display EC calibration recommendations based on K value
  Serial.println("Recommended calibration values based on K value:");
  Serial.println("For K10 probes:");
  Serial.println("  ec:cal,low,12880   -> Calibrate EC probe to 12,880us");
  Serial.println("  ec:cal,high,150000 -> Calibrate EC probe to 150,000us");
  Serial.println("For K1 probes:");
  Serial.println("  ec:cal,low,12880   -> Calibrate EC probe to 12,880us");
  Serial.println("  ec:cal,high,80000  -> Calibrate EC probe to 80,000us");
  Serial.println("For K0.1 probes:");
  Serial.println("  ec:cal,low,84      -> Calibrate EC probe to 84us");
  Serial.println("  ec:cal,high,1413   -> Calibrate EC probe to 1413us");
}

void loop() {
  char sensordata_buffer[32];

  // Send a command to read EC value
  EC.send_read_cmd();
  delay(1000);  // Wait for the sensor to process the command

  // Receive and print the EC value
  if (EC.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    float ec_value = String(sensordata_buffer).toFloat();
    Serial.print("EC: ");
    Serial.println(ec_value);
  } else {
    Serial.println("Error reading EC sensor.");
  }

  // Check if any calibration command is entered
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    EC.send_cmd(command.c_str());
  }

  delay(1000);  // Wait 1 second before the next reading
}