#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

void setup() {
    Serial.begin(115200);
    
    // Initialize INA219
    if (!ina219.begin()) {
        Serial.println("Failed to find INA219 chip");
        while (1);
    }

    Serial.println("INA219 Current Sensor Initialized.");
}

void loop() {
    float current_mA = ina219.getCurrent_mA(); // Read current in mA

    Serial.print("Current: ");
    Serial.print(current_mA);
    Serial.println(" mA");

    delay(1000); // Wait 1 second before next reading
}
