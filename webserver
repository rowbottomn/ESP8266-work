/*
  ESP8266_webserver_html.ino
  Nathan Rowbottom Nov 18 2020
  Code for the ESP8266 which allows the use to post up a web page.

  Also shows how to post the data from a sensor.
  
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


ESP8266WebServer server;//instantiating a server instance
uint8_t pin_led = 16;//unsigned int

//string literals which are stored in the read only memory to protect them.
const char* ssid = "STEAM Wifi";
const char* password = "workBOARDgoodbye95";

//storing the webpage as a char array in the flash memory instead of the ram to avoid memory limitations
char webpage[] PROGMEM = R"=====(
<html>
<head>
</head>
<body>
<h1>temperature and humidity</h1>
<form action="/update">
<button> Update </button>
</form>
</body>
</html>
)=====";//using this format to allow us to use newline characters to separate commands.  Other code online might use some special character and parse it directly

void toggleLED()
{
  //toggle the led to show response received 
  digitalWrite(pin_led,!digitalRead(pin_led));
}

void updateDate(){
  //TODO:  add code to get temperature/humidity  data from sensor
 
  toggleLED();//toggle led to show response
}

void setup()
{
  pinMode(pin_led, OUTPUT);
  WiFi.begin(ssid,password);
  Serial.begin(115200);

  //use a loop to hold the program from advancing until the connection has completed. 
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");//something to show progress updating 
    delay(500);
  }
  //
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/",[](){server.send(200,"text/html","<h1>SNP STEAM Academy!</h1>");});
  server.on("/update",updateData);
  server.begin();
}

void loop()
{
  server.handleClient();
}

