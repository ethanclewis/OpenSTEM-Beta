#include <Wire.h>
#include <Ezo_i2c.h>

// Define the EC sensor and pump
Ezo_board EC = Ezo_board(100, "EC");
Ezo_board PMP3 = Ezo_board(58, "PMP3");

float EC_UPPER_LIMIT = 2000.0;  // Initial upper threshold for EC value (microsiemens)
float PUMP_DOSE = -0.5;         // Hard-coded dose amount (mL)

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Enter new EC_UPPER_LIMIT value or press Enter to keep the current value.");
}

void loop() {
  // Check for user input to update EC_UPPER_LIMIT
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input.length() > 0) {
      float new_limit = input.toFloat();
      EC_UPPER_LIMIT = new_limit;  // Allow any float value, including negative
      Serial.print("EC_UPPER_LIMIT updated to: ");
      Serial.println(EC_UPPER_LIMIT);
    }
  }

  char sensordata_buffer[32];

  // Read and print EC value
  EC.send_read_cmd();
  delay(1000); // Wait for the sensor to process the command
  if (EC.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    float ec_value = String(sensordata_buffer).toFloat();
    Serial.print("EC: ");
    Serial.println(ec_value);

    // Trigger PMP3 if EC is above the upper limit
    if (ec_value > EC_UPPER_LIMIT) {
      Serial.println("EC is above the upper limit. Triggering PMP3...");
      PMP3.send_cmd_with_num("d,", PUMP_DOSE); // Dispense nutrient solution
    } else {
      Serial.println("EC is within acceptable range.");
    }
  } else {
    Serial.println("Error reading EC sensor.");
  }

  delay(5000); // Wait 5 seconds before next reading
}