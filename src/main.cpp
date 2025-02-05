#include <SPI.h>
#include <Wire.h>
#include <esp_sleep.h>

#define LM75A_ADDRESS 0x48  // Default I2C address of LM75A
#define TEMPERATURE_REGISTER 0x00
#define LED 4  // Pin for LED
#define WAKEUP_TIME 10  // Wake-up time in seconds

hw_timer_t *My_timer = NULL;  // Hardware timer pointer
volatile bool blinkEnabled = false;  // Flag to control LED blinking
float temperature_in_fahrenheit;

// Timer interrupt function - toggles LED every second (1 Hz)
void IRAM_ATTR onTimer() {
  if (blinkEnabled) {
    digitalWrite(LED, !digitalRead(LED));  // Toggle LED state
  }
}

// Function to read temperature from LM75A
float readTemperature() {
  Wire.beginTransmission(LM75A_ADDRESS);
  Wire.write(TEMPERATURE_REGISTER);
  Wire.endTransmission(false);  // Restart condition for read operation

  Wire.requestFrom(LM75A_ADDRESS, 2);
  if (Wire.available() == 2) {
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    
    int16_t rawTemperature = (msb << 8) | lsb;
    rawTemperature >>= 5;  // Right-align 11-bit value
    
    return rawTemperature * 0.125;  // Convert to Celsius
  }
  return -1000;  // Error value if data is unavailable
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(LED, OUTPUT);

  // Initialize hardware timer
  My_timer = timerBegin(1);  // Set frequency to 1 Hz (1 second period)
  timerAttachInterrupt(My_timer, &onTimer);  // Attach interrupt function
  timerAlarm(My_timer, 1000000, true, 0);  // Set alarm for 1 second

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
  temperature_in_fahrenheit = temperature_in_degrees * 1.8 + 32;

  if (temperature_in_degrees == -1000) {
    Serial.println("Error while getting temperature");
    blinkEnabled = false;
    digitalWrite(LED, LOW);
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature_in_degrees);
    Serial.print("°C (");
    Serial.print(temperature_in_fahrenheit);
    Serial.println("°F)");

    if (temperature_in_degrees < 16) {
      Serial.println("Temperature below 20°C. Entering deep sleep...");
      delay(1000);

      // Configure deep sleep
      esp_sleep_enable_timer_wakeup(WAKEUP_TIME * 1000000ULL);  // Wake up after WAKEUP_TIME seconds
      esp_deep_sleep_start();
    }
    // Light Sleep Mode if temperature is between 16°C and 17°C
    else if (temperature_in_degrees >= 16 && temperature_in_degrees < 17) {
      Serial.println("Temperature between 16°C and 17°C. Entering light sleep...");
      delay(1000);
      esp_sleep_enable_timer_wakeup(WAKEUP_TIME * 1000000ULL);  // Wake up after WAKEUP_TIME seconds
      esp_light_sleep_start();
    }
    
    // Normal Mode if temperature is 17°C or higher
    else {
      Serial.println("Temperature is normal. Running in active mode.");
      blinkEnabled = true;

  }
  }
  delay(2000);
}
