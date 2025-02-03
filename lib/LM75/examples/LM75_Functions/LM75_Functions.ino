#include <LM75.h>

LM75 Temp0;

float maxTemp, minTemp, Temp;

void setup() {
    Serial.begin(115200);
    while (!Serial);            // Leonardo: wait for serial monitor
    Temp0.begin();
    
    Serial.println();
    Serial.print("LM75 is ");

    if(Temp0.isConnected())
        Serial.println("Connected!");
    else
    {
        Serial.println("Not Connected");
        while(true);
    }

    Temp0.setMinTemperature(25);
    Temp0.setMaxTemperature(30);

    minTemp = Temp0.getMinTemperature();
    maxTemp = Temp0.getMaxTemperature();

    Serial.print("Min Temp : ");
    Serial.println(minTemp,1);
    Serial.print("Max Temp : ");
    Serial.println(maxTemp,1);


    //Temp0.getPolarity();
    Temp0.setPolarity(HIGH); // Now OS should be HIGH if temperature is between 25 to 30'C.

    //Temp0.shutDown();
    //Temp0.turnUp();
    //Temp0.getStatus();
   
    //Temp0.setOperationMode(COMPARATOR);
    //Temp0.setOperationMode(INTERRUPT);
    //Temp0.getOperationMode();
    

    //Temp0.setFaultQueue(1);
	//Temp0.getFaultQueue();
}

void loop() {
  // put your main code here, to run repeatedly:
    
    Serial.print(Temp0.getTemperature(),3);
    Serial.println("°C");
    delay(1000);
   
}
