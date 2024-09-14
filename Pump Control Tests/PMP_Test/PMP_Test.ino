#include <Wire.h>
#include <Ezo_i2c.h>

// Define the sensors and pumps
Ezo_board PH = Ezo_board(99, "PH");
Ezo_board EC = Ezo_board(100, "EC");
Ezo_board PMP1 = Ezo_board(56, "PMP1");
Ezo_board PMP2 = Ezo_board(57, "PMP2");
Ezo_board PMP3 = Ezo_board(58, "PMP3");

float PH_LOWER_LIMIT;
float PH_UPPER_LIMIT;
float EC_UPPER_LIMIT;
float PUMP_DOSE = -0.5;  // Hard-coded dose amount (mL)

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Prompt user to enter initial threshold values
  set_thresholds();
}

void loop() {
  // Check for user input to update thresholds
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();  // Remove any trailing whitespace or newlines
    if (input.equalsIgnoreCase("new")) {
      Serial.println("Entering new threshold values...");
      set_thresholds();
    }
  }

  // Execute sensor reading and pump control logic
  control_pumps();

  delay(5000); // Wait 5 seconds before next reading
}

// Function to set thresholds based on user input
void set_thresholds() {
  Serial.println("Enter PH_LOWER_LIMIT value:");
  PH_LOWER_LIMIT = read_float_from_serial();

  Serial.println("Enter PH_UPPER_LIMIT value:");
  PH_UPPER_LIMIT = read_float_from_serial();

  Serial.println("Enter EC_UPPER_LIMIT value:");
  EC_UPPER_LIMIT = read_float_from_serial();

  // Print the set threshold values for confirmation
  Serial.println("Thresholds set:");
  Serial.print("PH_LOWER_LIMIT: ");
  Serial.println(PH_LOWER_LIMIT);
  Serial.print("PH_UPPER_LIMIT: ");
  Serial.println(PH_UPPER_LIMIT);
  Serial.print("EC_UPPER_LIMIT: ");
  Serial.println(EC_UPPER_LIMIT);
}

// Function to read a float value from serial input
float read_float_from_serial() {
  while (true) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim();  // Remove any trailing whitespace or newlines
      float value = input.toFloat();
      return value;  // Accept 0 as a valid input
    }
  }
}

// Function to read sensors and control pumps based on thresholds
void control_pumps() {
  char sensordata_buffer[32];

  // Read and print pH value
  PH.send_read_cmd();
  delay(1000); // Wait for the sensor to process the command
  if (PH.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    float ph_value = String(sensordata_buffer).toFloat();
    Serial.print("pH: ");
    Serial.println(ph_value);

    // Trigger PMP1 if pH is below the lower limit
    if (ph_value < PH_LOWER_LIMIT) {
      Serial.println("pH is below the lower limit. Triggering PMP1...");
      PMP1.send_cmd_with_num("d,", PUMP_DOSE); // Dispense pH up solution
    }

    // Trigger PMP2 if pH is above the upper limit
    else if (ph_value > PH_UPPER_LIMIT) {
      Serial.println("pH is above the upper limit. Triggering PMP2...");
      PMP2.send_cmd_with_num("d,", PUMP_DOSE); // Dispense pH down solution
    } 

    // pH is within the acceptable range
    else {
      Serial.println("pH is within acceptable range.");
    }
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
}