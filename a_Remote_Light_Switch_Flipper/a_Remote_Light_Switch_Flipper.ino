#include "C_Libraries.h"
#include "B_HTML.h"

#define wifi_status_led 0
#define orientation_switch 4
#define button1 12
#define button2 13
#define ir_receiver 14
#define servo_pin 16

char* ssid = "NETGEAR37";
char* password = "364h-rbyu-lhtb";

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
