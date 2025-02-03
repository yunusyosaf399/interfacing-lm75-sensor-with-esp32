#include <LM75.h>

LM75 Temp0; //(0x48);

float maxTemp, minTemp, Temp;

void setup()
{
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

    Serial.print("Status : ");
    if(Temp0.getStatus() == ON)
        Serial.println(" ON");
    else 
        Serial.println("OFF");

    //Temp0.shutDown();
    //Temp0.turnUp();
    //Temp0.setOperationMode(COMPARATOR);
    //Temp0.setPolarity(LOW);

    //Temp0.setFaultQueue(1);

    //Temp0.setMinTemperature(20);
    //Temp0.setMaxTemperature(40);

    maxTemp = Temp0.getMaxTemperature();
    minTemp = Temp0.getMinTemperature();

    Serial.print("Min Temp : ");
    Serial.print(minTemp);
    Serial.println("°C");
    
    Serial.print("Max Temp : ");
    Serial.print(maxTemp);
    Serial.println("°C");

    Serial.print("Operation mode : ");
    if (Temp0.getOperationMode() == INTERRUPT)
        Serial.println("Interrupt");
    else
        Serial.println("Comparator");
        
    Serial.print("Polarity : ");
    if (Temp0.getPolarity() == LOW)
        Serial.println("Low");
    else
        Serial.println("High");

    Serial.print("Fault Queue : ");
    Serial.println(Temp0.getFaultQueue());
        
    Serial.println();
}

void loop()
{
    if(Temp0.getStatus() == ON)
    {
        Temp = Temp0.getTemperature();

        Serial.print("Current Temp : ");
        Serial.print(Temp,3);
        Serial.println("°C");
        delay(1000);
    }
}
