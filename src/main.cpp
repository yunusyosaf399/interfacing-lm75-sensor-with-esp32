#include <SPI.h>
#include <Wire.h>

#define LM75A_ADDRESS 0x48  // Default I2C address of LM75A
#define TEMPERATURE_REGISTER 0x00
#define LED 4  // Pin for LED

hw_timer_t *My_timer = NULL;  // Hardware timer pointer
volatile bool blinkEnabled = false;  // Flag to control LED blinking
float temperature_in_fahrenheit;
// Timer interrupt function - toggles LED every second (1 Hz)
void IRAM_ATTR onTimer() {
  if (blinkEnabled) {
    digitalWrite(LED, !digitalRead(LED));  // Toggle LED state
  }
}

// Function to read temperature directly from LM75A
float readTemperature() {
  Wire.beginTransmission(LM75A_ADDRESS);
  Wire.write(TEMPERATURE_REGISTER);
  Wire.endTransmission(false);  // Restart condition for read operation

  Wire.requestFrom(LM75A_ADDRESS, 2);
  if (Wire.available() == 2) {
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    
    // Convert to temperature (LM75A outputs in 0.125°C steps, left-aligned 9-bit format)
    int16_t rawTemperature = (msb << 8) | lsb;
    rawTemperature >>= 5;  // Right-align 11-bit value
    
    return rawTemperature * 0.125;  // Convert to Celsius
  }
  return -1000;  // Error value if data is unavailable
}

void setup() {
  Serial.begin(115200);
  Wire.begin();  // Start I2C communication
  pinMode(LED, OUTPUT);  // Set LED pin as output

  // Initialize hardware timer
  My_timer = timerBegin(1000000);            // Timer with 1-second interval
  timerAttachInterrupt(My_timer, &onTimer);  // Attach the interrupt handler
  timerAlarm(My_timer, 1000000, true, 0);    // Set alarm for 1 second (1000000 µs)

  Serial.println("Scanning for I2C devices...");
  for (byte i = 1; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Device found at address 0x");
      Serial.println(i, HEX);
    }
  }
}

void loop() {
  float temperature_in_degrees = readTemperature();
  temperature_in_fahrenheit = temperature_in_degrees*(9/5)+32;
  if (temperature_in_degrees == -1000) {
    Serial.println("Error while getting temperature");
    blinkEnabled = false;  // Disable LED blinking if data is invalid
    digitalWrite(LED, LOW); // Turn off LED
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature_in_degrees);
    Serial.print(" degrees");

    Serial.print(" (");
    Serial.print(temperature_in_degrees);
    Serial.println(" fahrenheit)");

    blinkEnabled = true;  // Enable LED blinking when data is valid
  }
}
