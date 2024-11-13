#include <Wire.h>
#include <Ezo_i2c.h>

// Define the pumps
Ezo_board PMP1 = Ezo_board(56, "PMP1");  // pH up
Ezo_board PMP2 = Ezo_board(57, "PMP2");  // pH down
Ezo_board PMP3 = Ezo_board(58, "PMP3");  // Nutrient pump

// Track the state of each pump
bool pmp1_running = false;
bool pmp2_running = false;
bool pmp3_running = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Enter pump number (1, 2, or 3) to toggle it on/off.");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    switch (command) {
      case '1':  // Toggle PMP1
        if (!pmp1_running) {
          PMP1.send_cmd("d,100");   // Start PMP1 with indefinite dispensing rate
          pmp1_running = true;
          Serial.println("PMP1 ON");
        } else {
          PMP1.send_cmd("d,0");     // Stop PMP1
          pmp1_running = false;
          Serial.println("PMP1 OFF");
        }
        break;

      case '2':  // Toggle PMP2
        if (!pmp2_running) {
          PMP2.send_cmd("d,100");   // Start PMP2 with indefinite dispensing rate
          pmp2_running = true;
          Serial.println("PMP2 ON");
        } else {
          PMP2.send_cmd("d,0");     // Stop PMP2
          pmp2_running = false;
          Serial.println("PMP2 OFF");
        }
        break;

      case '3':  // Toggle PMP3
        if (!pmp3_running) {
          PMP3.send_cmd("d,100");   // Start PMP3 with indefinite dispensing rate
          pmp3_running = true;
          Serial.println("PMP3 ON");
        } else {
          PMP3.send_cmd("d,0");     // Stop PMP3
          pmp3_running = false;
          Serial.println("PMP3 OFF");
        }
        break;

      default:
        Serial.println("Invalid input. Enter 1, 2, or 3 to toggle a pump.");
        break;
    }
  }

  delay(100);  // Short delay to avoid rapid serial checks
}