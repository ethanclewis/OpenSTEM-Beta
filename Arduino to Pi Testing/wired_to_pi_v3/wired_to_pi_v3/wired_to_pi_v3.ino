#include <Wire.h>
#include <Ezo_i2c.h>

// Define the sensors
Ezo_board PH = Ezo_board(99, "PH");
Ezo_board EC = Ezo_board(100, "EC");
Ezo_board RTD = Ezo_board(102, "RTD");

// Timer variables
unsigned long previousMillis = 0;
const long interval = 600000;  // 10 minutes (600000 ms)

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  // Current time for timing-based logic
  unsigned long currentMillis = millis();

  // Poll sensors every 10 minutes
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Reset timer
    poll_sensors();  // Read and print sensor values
  }

  delay(1000);  // Wait for 1 second before checking again
}

// Function to poll sensors and print readings
void poll_sensors() {
  float ph_value = read_ph();
  float ec_value = read_ec();
  float temp_value = read_temp();

  // Print sensor values to the serial monitor
  Serial.print("pH: ");
  Serial.println(ph_value);
  Serial.print("EC: ");
  Serial.println(ec_value);
  Serial.print("Temperature: ");
  Serial.println(temp_value);
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