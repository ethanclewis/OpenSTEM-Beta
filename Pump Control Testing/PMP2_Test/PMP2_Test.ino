#include <Wire.h>
#include <Ezo_i2c.h>

// Define the pH sensor and pump
Ezo_board PH = Ezo_board(99, "PH");
Ezo_board PMP2 = Ezo_board(57, "PMP2");

float PH_UPPER_LIMIT = 7.0;  // Initial upper threshold for pH value
float PUMP_DOSE = -0.5;      // Hard-coded dose amount (mL)

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Enter new PH_UPPER_LIMIT value or press Enter to keep the current value.");
}

void loop() {
  // Check for user input to update PH_UPPER_LIMIT
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input.length() > 0) {
      float new_limit = input.toFloat();
      if (new_limit > 0) {
        PH_UPPER_LIMIT = new_limit;
        Serial.print("PH_UPPER_LIMIT updated to: ");
        Serial.println(PH_UPPER_LIMIT);
      } else {
        Serial.println("Invalid input. PH_UPPER_LIMIT not changed.");
      }
    }
  }

  char sensordata_buffer[32];

  // Read and print pH value
  PH.send_read_cmd();
  delay(1000); // Wait for the sensor to process the command
  if (PH.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    float ph_value = String(sensordata_buffer).toFloat();
    Serial.print("pH: ");
    Serial.println(ph_value);

    // Trigger PMP2 if pH is above the upper limit
    if (ph_value > PH_UPPER_LIMIT) {
      Serial.println("pH is above the upper limit. Triggering PMP2...");
      PMP2.send_cmd_with_num("d,", PUMP_DOSE); // Dispense pH down solution
    } else {
      Serial.println("pH is within acceptable range.");
    }
  } else {
    Serial.println("Error reading pH sensor.");
  }

  delay(5000); // Wait 5 seconds before next reading
}