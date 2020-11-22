#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
//#include <Parsing-impl.h>
#include <Uri.h>
#include <ESP8266WiFi.h>

const char* ssid = "CGN3-4DD8";
const char* password = "251149021463";

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(88);

String webSite, JSONtxt;
unsigned long websocketCount = 0 , wait = 0;

const int LED = 2; 
//int websockMillis = 50;

boolean LEDonoff = true;

const char webSiteCont[] PROGMEM = 
R"=====(
<!DOCTYPE HTML>
<html>
  <style>
  #btn{
    display : inline-block;
    text-decoration:none;
    background: #470469;
    color : rgba(255,255,255,0.80);
    width : 320px;
    height : 320px;
    border-radius: 20%;
    margin:30%;
    margin-top: 60px;
    text-align:center; 
    vertical-align: middle;
    overflow:hidden;
    box-shadow:0px 0px 0px 8px #470469;
    border: solid 2px rgba(150,150,150,0.33);
    transition: 0.4s;
  }
  </style>
  <script>
    var websock;

    InitWebSocket();
    
    function InitWebSocket()
    {
      websock = new WebSocket('ws://'+window.location.hostname+':88/');
      btn = document.getElementById('btn');
      websock.onmessage=function(evt)
      {
        var JSONobj = JSON.parse(evt.data);
        var b = document.getElementById('btn');
        b.innerHTML = JSONobj.LEDonoff;
        
        if (JSONobj.LEDonoff == 'ON')
        {
          b.style.background = '#501899';  
          b.style["boxShadow"] = '0px 0px 0px 8px #501899';
        }
        else 
        {
          b.style.background = '#470469';  
          b.style["boxShadow"] = '0px 0px 0px 8px #470469';
        }
      }//end onmessage evt
    }//end initwebsocket
  
    function button()
    {
      var b = document.getElementById('btn');
      var btnResponse = 'LEDonoff=ON';
      
      if (b.innerHTML === 'ON')
      {
        btnResponse = 'LEDonoff=OFF';
      }
  
      websock.send(btnResponse);
      
    }
    
     
  </script>
  
  <head>
  </head>
  <body>
  <h1>temperature and humidity</h1>
  
  <a id = "btn" ONCLICK = "button()"> Update </a>
  
  </body>
</html>
  
)=====";

void doWebSite(){
  server.send(200, "text/html", webSiteCont);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t wslength)
{
  String payloadString = (const char *)payload;
  Serial.print("payloadString= ");
  Serial.print(payloadString);

  if (type == WStype_TEXT)//if the client receives text
  {
    byte separator = payloadString.indexOf('=');//find the position of the equal sign in the text
    String var = payloadString.substring(0, separator);//get before the equal sign
    Serial.print("var = ");
    Serial.println(var);
    String val = payloadString.substring(separator+1);//get after the equal sign
    Serial.print("val = ");
    Serial.println(val);

    if (var == "LEDonoff")
    {
      if (val == "OFF")
      {
        LEDonoff = false;
      }
      else if (val == "ON")
      {
        LEDonoff = true;
      }
      else
      {
        Serial.print("val = WTF");
      }
      
    }
  } 
  
}

void setup() 
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  WiFi.begin(ssid, password);

  //wait for the connection
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }

  WiFi.mode(WIFI_STA);
  Serial.println("Start ESP ");
  Serial.println(WiFi.localIP());
  server.on("/", doWebSite);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() 
{
  webSocket.loop();
  server.handleClient();

  if (LEDonoff)
  {
    digitalWrite(LED, LOW);
  }
  else
  {
    digitalWrite(LED, HIGH);
  }

 // if (millis() > wait)
 // {
    //websocketCount ++;
    String LEDswitch = "OFF";
    if (LEDonoff == true){
      LEDswitch = "ON";
    }
    JSONtxt = "{\"LEDonoff\":\""+LEDswitch+"\"}";
    webSocket.broadcastTXT(JSONtxt);
 // }
}
