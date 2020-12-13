#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <Servo.h>

#define ir_receiver 14
#define ir_up 0xFF629D
#define ir_down 0xFFA857
#define servo_pin 16

IRrecv irrecv(ir_receiver);
Servo myservo;

decode_results results;

void setup() {
  Serial.begin(115200);
  
  irrecv.enableIRIn();  // Start the receiver
  pinMode(servo_pin, OUTPUT);
  myservo.attach(servo_pin);
  
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(ir_receiver);

  myservo.write(85);
  delay(500);
}

void loop() {
  myservo.write(85);
  
  if (irrecv.decode(&results))// Returns 0 if no data ready, 1 if data ready. 
  {
    Serial.print("Code: "); 
    Serial.println(results.value, HEX); //prints the value a a button press

    switch (results.value)
    {
      case upButton:
          myservo.write(115);
          delay(500);
          break;
          
      case downButton:
          myservo.write(55);
          delay(500);
          break;

      default: myservo.write(100);
    }
    
    irrecv.resume(); //prepare to get next value
  }
}
