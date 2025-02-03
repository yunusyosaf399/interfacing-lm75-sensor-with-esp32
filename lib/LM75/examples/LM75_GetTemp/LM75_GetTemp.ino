/*
    Sketch to get temperature from LM75 Without library!
    Why not?
*/

#include <Wire.h>

uint8_t temp_msb, temp_lsb;
int temp_address = 0x48;
uint16_t temp_reg;
float temperatureC, temperatureF;

void setup() {
  
    Serial.begin(115200);
    Serial.println();
    Wire.begin(); 
}

void loop() {

    Wire.beginTransmission(temp_address);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.requestFrom(temp_address, 2);
    temp_msb = Wire.read();
    temp_lsb = Wire.read();

    temp_reg = (temp_msb << 8) | temp_lsb;

    temperatureC = temp_reg/256.0f;
    temperatureF = (temperatureC * 9/5) + 32;

    Serial.print("Temperature in C : ");
    Serial.print(temperatureC,3);
    Serial.print("°C");

    Serial.print("Temperature in F : ");
    Serial.print(temperatureF,3);
    Serial.println("°F");

    delay(2000);

}
