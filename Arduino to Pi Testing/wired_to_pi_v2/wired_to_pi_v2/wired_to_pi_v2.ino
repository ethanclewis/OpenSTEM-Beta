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
unsigned long previousSensorMillis = 0;
unsigned long previousPumpMillis = 0;

// Polling every 10 minutes (10 * 60 * 1000 ms)
const long sensorInterval = 600000;  // 10 minutes

// Pump logic every 30 minutes (30 * 60 * 1000 ms)
const long pumpInterval = 1800000;   // 30 minutes

// Variables to store the latest sensor readings
float last_ph = 0.0, last_ec = 0.0, last_temp = 0.0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  // Current time for timing-based logic
  unsigned long currentMillis = millis();

  // ---- Poll sensors every 10 minutes ----
  if (currentMillis - previousSensorMillis >= sensorInterval) {
    previousSensorMillis = currentMillis;  // Reset sensor timer
    poll_sensors();  // Read and print sensor values
  }

  // ---- Run pump control logic every 30 minutes ----
  if (currentMillis - previousPumpMillis >= pumpInterval) {
    previousPumpMillis = currentMillis;  // Reset pump control timer
    control_pumps(last_ph, last_ec);  // Adjust pumps based on the last readings
  }

  delay(1000);  // Wait for 1 second before looping again
}

// Function to poll sensors and store readings
void poll_sensors() {
  last_ph = read_ph();
  last_ec = read_ec();
  last_temp = read_temp();

  // Print sensor values to the serial monitor
  Serial.print("pH: ");
  Serial.println(last_ph);
  Serial.print("EC: ");
  Serial.println(last_ec);
  Serial.print("Temperature: ");
  Serial.println(last_temp);
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

// Function to control pumps based on pH and EC thresholds
void control_pumps(float ph_value, float ec_value) {
  // ---- pH control logic ----
  if (ph_value < PH_LOWER_LIMIT) {
    Serial.println("pH is below the lower limit. Triggering PMP1...");
    PMP1.send_cmd_with_num("d,", PUMP_DOSE);  // Dispense pH up solution
  } else if (ph_value > PH_UPPER_LIMIT) {
    Serial.println("pH is above the upper limit. Triggering PMP2...");
    PMP2.send_cmd_with_num("d,", PUMP_DOSE);  // Dispense pH down solution
  } else {
    Serial.println("pH is within acceptable range.");
  }

  // ---- EC control logic ----
  if (ec_value < EC_LOWER_LIMIT) {
    Serial.println("EC is below the lower limit. Triggering PMP3...");
    PMP3.send_cmd_with_num("d,", PUMP_DOSE);  // Dispense nutrient solution
  } else if (ec_value > EC_UPPER_LIMIT) {
    Serial.println("EC is above the upper limit. No pump action, notification sent.");
  } else {
    Serial.println("EC is within acceptable range.");
  }
}