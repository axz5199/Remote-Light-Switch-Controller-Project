/*
Black button = button 1 (up)
Green button = button 2 (down)
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

ESP8266WebServer server;

#define pin_led 0
#define button1 12
#define button2 13
#define servo_pin 16

Servo myservo;

char* ssid = "NETGEAR37";
char* password = "364h-rbyu-lhtb";

char index_html[] PROGMEM = {"<html>\n"
"  <head>\n"
"\t<style>\n"
"\t\tbody {\n"
"\t\t  background-color: #212226;\n"
"\t\t}\n"
"\n"
"\t\t.buttons {\n"
"\t\t  text-align: center;\n"
"\t\t}\n"
"\n"
"\t\t.button1 {\n"
"\t\t  color: black;\n"
"\t\t  background-color: #23c48e;\n"
"\t\t  font-size: 50px;\n"
"\t\t  width: 180px;\n"
"\t\t  border: none;\n"
"\t\t  border-radius: 20px;\n"
"\t\t  padding: 15px 30px;\n"
"\t\t  transform: translate(0px, 200px);\n"
"\t\t  cursor: pointer;\n"
"\t\t}\n"
"\n"
"\t\t.button2 {\n"
"\t\t  color: black;\n"
"\t\t  background-color: #23c48e;\n"
"\t\t  font-size: 50px;\n"
"\t\t  width: 180px;\n"
"\t\t  border: none;\n"
"\t\t  border-radius: 20px;\n"
"\t\t  padding: 15px 30px;\n"
"\t\t  transform: translate(0px, 200px);\n"
"\t\t  cursor: pointer;\n"
"\t\t}\n"
"\n"
"\t\t.button1:hover {\n"
"\t\t  background-color: #1fad7e\n"
"\t\t}\n"
"\n"
"\t\t.button1:active {\n"
"\t\t  transform: translateY(190px);\n"
"\t\t}\n"
"\n"
"\t\t.button2:hover {\n"
"\t\t  background-color: #1fad7e\n"
"\t\t}\n"
"\n"
"\t\t.button2:active {\n"
"\t\t  transform: translateY(210px);\n"
"\t\t}\n"
"\t</style>\n"
"\t<title>Remote Light Switch</title>\n"
"\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
"  </head>\n"
"  <body class=\"buttons\">\n"
"   <form action=\"/up\">\n"
"    <button class=\"button1\"> &#8679 </button>\n"
"   </form>\n"
"   <form action=\"/down\">\n"
"    <button class=\"button2\"> &#8681 </button>\n"
"   </form>\n"
"  </body>\n"
"</html>"};

void setup()
{
  //Pinmodes
  pinMode(pin_led, OUTPUT); // Built-in led pin
  pinMode(servo_pin, OUTPUT); // Servo control pin
  pinMode(button1, INPUT_PULLUP); //Button input pin
  pinMode(button2, INPUT_PULLUP); //Button input pin

  //Setup
  Serial.begin(115200);
  myservo.attach(servo_pin); //Servo setup
  WiFi.begin(ssid, password); //Wifi setup

  //Connect to wifi
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  //Establish HTML routes
  server.on("/", handleRoot);
  server.on("/up", handleUp);
  server.on("/down", handleDown);
  server.begin();
  
  Serial.println(""); Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
}

void loop()
{
  server.handleClient();

  myservo.write(85);

  if (digitalRead(button1) == 0)
  {
    Serial.println("Up pressed");
    myservo.write(120);
    delay(500);
  }

  if (digitalRead(button2) == 0)
  {
    Serial.println("Down pressed");
    myservo.write(45);
    delay(500);
  }
}
