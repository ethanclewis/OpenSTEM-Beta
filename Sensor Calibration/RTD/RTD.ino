#include <Wire.h>
#include <Ezo_i2c.h>

// Define the RTD sensor
Ezo_board RTD = Ezo_board(102, "RTD");

void setup() {
  Serial.begin(9600);   // Start serial communication
  Wire.begin();         // Start I2C communication
  
  Serial.println("RTD Sensor Calibration");
  Serial.println("Commands:");
  Serial.println("rtd:cal,[temperature] - Calibrate the RTD sensor to a known temperature.");
  Serial.println("rtd:cal,clear - Clear the RTD calibration.");
  Serial.println("Example: To calibrate at 70°F (21.1°C), send 'rtd:cal,21.1'.");
}

void loop() {
  char sensordata_buffer[32];

  // Read and print the RTD value
  RTD.send_read_cmd();
  delay(1000); // Wait for the sensor to process the command
  if (RTD.receive_cmd(sensordata_buffer, sizeof(sensordata_buffer)) == Ezo_board::SUCCESS) {
    float temp_value = String(sensordata_buffer).toFloat();
    Serial.print("Temperature: ");
    Serial.print(temp_value);
    Serial.println("°C");
  } else {
    Serial.println("Error reading RTD sensor.");
  }

  delay(1000); // Wait 1 second before next reading
}