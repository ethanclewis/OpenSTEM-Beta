// Polls and prints sensor readings every 1 second
// Pump control every 1 minute

#include <Wire.h>
#include <Ezo_i2c.h>

// Define the sensors and pumps
Ezo_board PH = Ezo_board(99, "PH");
Ezo_board EC = Ezo_board(100, "EC");
Ezo_board RTD = Ezo_board(102, "RTD");
Ezo_board PMP1 = Ezo_board(56, "PMP1");
Ezo_board PMP2 = Ezo_board(57, "PMP2");
Ezo_board PMP3 = Ezo_board(58, "PMP3");

// Pump control thresholds
float PH_LOWER_LIMIT = 5.5;
float PH_UPPER_LIMIT = 6.5;
float EC_LOWER_LIMIT = 1000.0;
float EC_UPPER_LIMIT = 2000.0;
float PUMP_DOSE = -0.5;  // Hard-coded dose amount (mL)

// Timer variables
unsigned long previousMillis = 0;
const long interval = 60000;  // 1 minute

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  // Current time for timing-based logic
  unsigned long currentMillis = millis();

  // Sensor readings
  float ph_value = read_ph();
  float ec_value = read_ec();
  float temp_value = read_temp();

  // ---- Print sensor values in the correct format for the Python script ----
  Serial.print("pH: ");
  Serial.println(ph_value);
  Serial.print("EC: ");
  Serial.println(ec_value);
  Serial.print("Temperature: ");
  Serial.println(temp_value);

  // ---- Every minute, check pump control logic ----
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Reset timer
    control_pumps(ph_value, ec_value);
  }

  delay(1000);  // Wait for 1 second before the next reading
}

// Function to read pH value
float read_ph() {
  char sensordata_buffer[32];
  PH.send_read_cmd();
  delay(1000); // Wait for the sensor to process the command
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
  delay(1000); // Wait for the sensor to process the command
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
  delay(1000); // Wait for the sensor to process the command
  if (RTD.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    return String(sensordata_buffer).toFloat();
  } else {
    Serial.println("Error reading Temperature sensor.");
    return -1;
  }
}

// Function to control pumps based on pH and EC thresholds
void control_pumps(float ph_value, float ec_value) {
  // ---- pH control logic ----
  // Trigger PMP1 if pH is below the lower limit
  if (ph_value < PH_LOWER_LIMIT) {
    Serial.println("pH is below the lower limit. Triggering PMP1...");
    PMP1.send_cmd_with_num("d,", PUMP_DOSE); // Dispense pH up solution
  }
  
  // Trigger PMP2 if pH is above the upper limit
  else if (ph_value > PH_UPPER_LIMIT) {
    Serial.println("pH is above the upper limit. Triggering PMP2...");
    PMP2.send_cmd_with_num("d,", PUMP_DOSE); // Dispense pH down solution
  } else {
    Serial.println("pH is within acceptable range.");
  }

  // ---- EC control logic ----
  // Trigger PMP3 if EC is below the lower limit
  if (ec_value < EC_LOWER_LIMIT) {
    Serial.println("EC is below the lower limit. Triggering PMP3...");
    PMP3.send_cmd_with_num("d,", PUMP_DOSE); // Dispense nutrient solution
  }
  
  // Print a notification if EC is above the upper limit
  else if (ec_value > EC_UPPER_LIMIT) {
    Serial.println("EC is above the upper limit. No pump action, notification sent.");
  } else {
    Serial.println("EC is within acceptable range.");
  }
}