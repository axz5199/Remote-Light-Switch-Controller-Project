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

  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");

  myservo.write(85);
  delay(500);
}

void loop() {
  myservo.write(85);
  
  if (irrecv.decode(&results)) {

    serialPrintUint64(results.value, HEX);
    Serial.println("");

    switch (results.value)
    {
      case ir_up:
        Serial.println("up");
        myservo.write(120);
        delay(500);
        break;

      case ir_down:
        Serial.println("down");
        myservo.write(45);
        delay(500);
        break;

      default: myservo.write(85);
    }

    irrecv.resume();  // Receive the next value
  }
}
