#include <Wire.h>
#include <Ezo_i2c.h>

// Define the sensors and pumps
Ezo_board PH = Ezo_board(99, "PH");
Ezo_board EC = Ezo_board(100, "EC");
Ezo_board RTD = Ezo_board(102, "RTD");
Ezo_board PMP1 = Ezo_board(56, "PMP1");  // pH up
Ezo_board PMP2 = Ezo_board(57, "PMP2");  // pH down

// Thresholds for pump control
float PH_LOWER_LIMIT = 5.5;
float PH_UPPER_LIMIT = 6.5;
float EC_LOWER_LIMIT = 1000.0;
float EC_UPPER_LIMIT = 2000.0;
float PUMP_DOSE = -0.5;  // Example dose

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Arduino Ready");  // Signal that Arduino is ready
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'R') {  // 'R' for Read sensors
      poll_sensors();
    }
    else if (command == 'P') {  // 'P' for Pump control
      control_pumps();
    }
  }
  delay(100);  // Small delay for stability
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

// Function to control pumps based on thresholds
void control_pumps() {
  float ph_value = read_ph();

  // pH control logic
  if (ph_value < PH_LOWER_LIMIT) {
    Serial.println("pH below limit. Triggering PMP1...");
    PMP1.send_cmd_with_num("d,", PUMP_DOSE);  // Dispense pH up
  } else if (ph_value > PH_UPPER_LIMIT) {
    Serial.println("pH above limit. Triggering PMP2...");
    PMP2.send_cmd_with_num("d,", PUMP_DOSE);  // Dispense pH down
  } else {
    Serial.println("pH is within range.");
  }
}

// Function to read pH value
float read_ph() {
  char sensordata_buffer[32];
  PH.send_read_cmd();
  delay(1000);  // Wait for sensor response
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
  delay(1000);
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
  delay(1000);
  if (RTD.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    return String(sensordata_buffer).toFloat();
  } else {
    Serial.println("Error reading Temperature sensor.");
    return -1;
  }
}