/*
Black button = button 1 (up)
Green button = button 2 (down)
*/
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

#define wifi_status_led 0
#define orientation_switch 4
#define button1 12
#define button2 13
#define ir_receiver 14
#define servo_pin 16

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

#define ir_up 0xFF629D
#define ir_down 0xFFA857

int wifi_connection_timer = 0; //Measures the amount of time spent trying to connect to wifi

int servo_position_up = 130;
int servo_position_mid = 90;
int servo_position_down = 50;

ESP8266WebServer server;
Servo myservo;
IRrecv irrecv(ir_receiver);
decode_results results;

void setup()
{
  //Pinmodes
  pinMode(wifi_status_led, OUTPUT); // Built-in led pin
  pinMode(servo_pin, OUTPUT); // Servo control pin
  pinMode(button1, INPUT_PULLUP); //Button input pin
  pinMode(button2, INPUT_PULLUP); //Button input pin
  pinMode(orientation_switch, INPUT_PULLUP); //Sliding switch input pin

  //Esablish orientation
  if (digitalRead(orientation_switch) == 0) //Servo on left side
  {
    servo_position_up = 50;
    servo_position_down = 130;
  }
  else //Servo on right side
  {
    servo_position_up = 130;
    servo_position_down = 50;
  }
  

  //Setup
  Serial.begin(115200); delay(15); Serial.println("");
  myservo.attach(servo_pin); //Servo setup
  myservo.write(servo_position_mid);
  irrecv.enableIRIn(); //Start the receiver
  WiFi.begin(ssid, password); //Wifi setup
  
  //Connect to wifi
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(wifi_status_led, LOW);
    delay(500);
    Serial.print(".");
    digitalWrite(wifi_status_led, HIGH);
    delay(500);
    wifi_connection_timer++;
    if (wifi_connection_timer == 10) break;
  }

  Serial.println("");

  //Establish HTML routes if wifi successfully connects
  if (WiFi.status()==WL_CONNECTED)
  {
    server.on("/", handleRoot); //Call function to handle root
    server.on("/up", handleUp); //Call function to handle up command
    server.on("/down", handleDown); //Call function to handle down command
    server.begin(); //Start server
    Serial.print("Connected to "); Serial.println(ssid);
    Serial.print("IP Address: "); Serial.println(WiFi.localIP());
    digitalWrite(wifi_status_led, LOW);
  }
  else //Skip wifi setup if it fails to connect
  {
    Serial.println("Wifi failed to connect");
    digitalWrite(wifi_status_led, HIGH);
  }
  myservo.detach();
}

void loop()
{
  delay(1);
  
  //Wifi control
  server.handleClient();

  //Physical button control
  if (digitalRead(button1) == 0)
  {
    Serial.println("PB up");
    myservo.attach(servo_pin);
    myservo.write(130);
    delay(500);
    myservo.write(90);
    delay(500);
    myservo.detach();
  }

  if (digitalRead(button2) == 0)
  {
    Serial.println("PB down");
    myservo.attach(servo_pin);
    myservo.write(50);
    delay(500);
    myservo.write(90);
    delay(500);
    myservo.detach();
  }

  //IR control
  if (irrecv.decode(&results)) {

    serialPrintUint64(results.value, HEX);
    Serial.print(" ");

    switch (results.value)
    {
      case ir_up:
        Serial.println("IR up");
        myservo.attach(servo_pin);
        myservo.write(servo_position_up);
        delay(500);
        myservo.write(servo_position_mid);
        delay(500);
        myservo.detach();
        break;

      case ir_down:
        Serial.println("IR down");
        myservo.attach(servo_pin);
        myservo.write(servo_position_down);
        delay(500);
        myservo.write(servo_position_mid);
        delay(500);
        myservo.detach();
        break;

      default:
        Serial.println("IR misc");
    }
    irrecv.resume();  //Receive the next value
  }
}
