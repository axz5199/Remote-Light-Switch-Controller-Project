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

#define ir_up 0xFF629D          //IR code for up button on Keyes IR remote
#define ir_down 0xFFA857        //IR code for down button on Keyes IR remote

int wifi_connection_timer = 0; //Measures the amount of time spent trying to connect to wifi

int servo_position_up = 130;  //130 deg is up when servo motor is attached on the right side
int servo_position_mid = 90;  //90 deg is default middle position
int servo_position_down = 50; //50 deg is down when servo motor is attached on the left side

ESP8266WebServer server;
Servo myservo;
IRrecv irrecv(ir_receiver);
decode_results results;

void setup()
{
  //PINMODES//
  pinMode(wifi_status_led, OUTPUT);          // Built-in led pin
  pinMode(servo_pin, OUTPUT);                // Servo control pin
  pinMode(button1, INPUT_PULLUP);            //Button input pin
  pinMode(button2, INPUT_PULLUP);            //Button input pin
  pinMode(orientation_switch, INPUT_PULLUP); //Sliding switch input pin
  //PINMODES//


  //ESTABLISH ORIENTATION//
  if (digitalRead(orientation_switch) == 0) //When servo on left side
  {
    servo_position_up = 50;
    servo_position_down = 130;
  }
  else                                      //When servo on right side
  {
    servo_position_up = 130;
    servo_position_down = 50;
  }
  //ESTABLISH ORIENTATION//

  
  //ENABLE HARDWARE AND WIFI//
  Serial.begin(115200); delay(15); Serial.println("");
  myservo.attach(servo_pin);              //Attach servo
  myservo.write(servo_position_mid);      //Set servo arm to default position
  irrecv.enableIRIn();                    //Start the receiver
  WiFi.begin(ssid, password);             //Enter wifi credentials
  //ENABLE HARDWARE AND WIFI//

  
  //CONNECT TO WIFI//
  while(WiFi.status()!=WL_CONNECTED)        //Loop while wifi is attempting to connect
  {
    Serial.print(".");                      //Indicates loading
    digitalWrite(wifi_status_led, LOW);     //Turn on led attached to pin 0
    delay(500);
    Serial.print(".");                      //Indicates loading
    digitalWrite(wifi_status_led, HIGH);    //Turn off led attached to pin 0
    delay(500);
    wifi_connection_timer++;                //Increments counter every 1 second
    if (wifi_connection_timer == 10) break; //Stop attempting to connect to wifi if unsuccessful after 10 seconds
  }
  //CONNECT TO WIFI//


  Serial.println(""); //Skip line after loading dots
  

  //ESTABLISH HTML ROUTES//
  if (WiFi.status()==WL_CONNECTED)        //Run only when wifi is connected
  {
    server.on("/", handleRoot);           //Call function to handle root
    server.on("/up", handleUp);           //Call function to handle up command
    server.on("/down", handleDown);       //Call function to handle down command
    server.begin();                       //Start server
    Serial.print("Connected to ");  
    Serial.println(ssid);                 //Print network name
    Serial.print("IP Address: ");   
    Serial.println(WiFi.localIP());       //Print network IP address
    digitalWrite(wifi_status_led, LOW);   //Wifi indicator led steady on when connected
  }
  else                                    //Skip wifi setup if it fails to connect
  {
    Serial.println("Wifi failed to connect");
    digitalWrite(wifi_status_led, HIGH);  //Wifi indicator led off when no wifi connection
  }
  //ESTABLISH HTML ROUTES//

  myservo.detach(); //Turn off servo to conserve power
}
//END setup()

void loop()
{
  delay(1); //Allows ESP8266 to modem sleep
  
  //WIFI CONTROL//
  server.handleClient();
  //WIFI CONTROL//


  //PUSH BUTTON CONTROL//
  if (digitalRead(button1) == 0)  //When the up button is pressed
  {
    Serial.println("PB up");
    myservo.attach(servo_pin);    //Attach servo motor
    myservo.write(130);           //Move arm up
    delay(500);                   //Provides sufficient time for arm to move
    myservo.write(90);            //Move arm back to mid
    delay(500);                   //Provides sufficient time for arm to move
    myservo.detach();             //Detach servo motor to conserve power
  }

  if (digitalRead(button2) == 0)  //When the down button is pressed
  {
    Serial.println("PB down");
    myservo.attach(servo_pin);    //Attach servo motor
    myservo.write(50);            //Move arm down
    delay(500);                   //Provides sufficient time for arm to move
    myservo.write(90);            //Move arm back to mid
    delay(500);                   //Provides sufficient time for arm to move
    myservo.detach();             //Detach servo motor to conserve power
  }
  //PUSH BUTTON CONTROL//
  

  //IR CONTROL//
  if (irrecv.decode(&results))              //If ir receiver obtains a signal
  {
    serialPrintUint64(results.value, HEX);
    Serial.print(" ");

    switch (results.value)                  //Check ir code
    {
      case ir_up:                           //When up code (0xFF629D)
        Serial.println("IR up");
        myservo.attach(servo_pin);          //Attach servo motor
        myservo.write(servo_position_up);   //Move arm up
        delay(500);                         //Provides sufficient time for arm to move
        myservo.write(servo_position_mid);  //Move arm back to mid
        delay(500);                         //Provides sufficient time for arm to move
        myservo.detach();                   //Detach servo motor to conserve power
        break;

      case ir_down:                         //When down code (0xFFA857)
        Serial.println("IR down");
        myservo.attach(servo_pin);          //Attach servo motor
        myservo.write(servo_position_down); //Move arm down
        delay(500);                         //Provides sufficient time for arm to move
        myservo.write(servo_position_mid);  //Move arm back to mid
        delay(500);                         //Provides sufficient time for arm to move
        myservo.detach();                   //Detach servo motor to conserve power
        break;

      default:                              //When code is undefined
        Serial.println("IR misc");
    }
    
    irrecv.resume();                        //Receive the next code
  }
  //IR CONTROL//
}
//END loop()
