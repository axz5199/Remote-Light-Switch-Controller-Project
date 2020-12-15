void handleRoot()
{
  server.send_P(200, "text/html", index_html);
}

void handleUp()
{
  Serial.println("Wifi up");
  myservo.attach(servo_pin);
  myservo.write(servo_position_up);
  server.send_P(200, "text/html", index_html);
  delay(500);
  myservo.write(servo_position_mid);
  delay(500);
  myservo.detach();
}

void handleDown()
{
  Serial.println("Wifi down");
  myservo.attach(servo_pin);
  myservo.write(servo_position_down);
  server.send_P(200, "text/html", index_html);
  delay(500);
  myservo.write(servo_position_mid);
  delay(500);
  myservo.detach();
}
