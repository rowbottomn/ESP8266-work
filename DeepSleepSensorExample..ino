#include <dummy.h>

/*
 * ESP8266_DHT11_TempHumiditySensorExample
 * Nathan Rowbottom Nov 17 2020
 * Uses a DHT11 to read temp and humdity sensor to output the readings
 * note that the board should be nodemcu 1.0 12-E
 */


#include "DHTesp.h"

DHTesp dht;

int sleepTime = 3000;

void reportData(){
    
    if (dht.getStatusString()){
      float humidity = dht.getHumidity();
      float temperature = dht.getTemperature();
      Serial.print("\t");
      Serial.print(humidity, 0);
      Serial.print("\t\t");
      Serial.print(temperature, 1);
      Serial.print("\t\t");
      Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
      Serial.println("\t\t");
    }
    else{
      Serial.println("\tSENSOR NOT WORKING!\t");    
    }

}

const int DHT_PIN = 5;//

void setup()
{
  Serial.begin(9600);
  dht.setup(DHT_PIN, DHTesp::DHT11); // Connect DHT sensor to GPIO 17

  // Wait for serial to initialize.
  while(!Serial) { }
  Serial.flush();
  delay(sleepTime);
  reportData();
  // Deep sleep mode for 3 seconds, the ESP8266 wakes up by itself when GPIO 16 (D0 in NodeMCU board) is connected to the RESET pin
  Serial.println("I'm awake, but I'm going into deep sleep mode for 3 seconds");
  ESP.deepSleep(3e6); 
  //verified that this will never run
  //Serial.println("Why am I still I awake?!");
  
}


 //this never runs
void loop()
{
}
