/*
Black button = button 1 (up)
Green button = button 2 (down)
*/
#include <Servo.h>

#define button1 12
#define button2 13
#define servo_pin 16

Servo myservo;

void setup() {
  Serial.begin(115200);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(servo_pin, OUTPUT);
  myservo.attach(servo_pin);
  
  myservo.write(85);
  delay(500);
  
  Serial.println("Ready to Go!");
}

void loop() {
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
