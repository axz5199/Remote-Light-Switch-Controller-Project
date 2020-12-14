void handleRoot()
{
  server.send_P(200, "text/html", index_html);
}

void handleUp()
{
  Serial.println("Wifi up");
  myservo.write(servo_position_up);
  server.send_P(200, "text/html", index_html);
  delay(500);
}

void handleDown()
{
  Serial.println("Wifi down");
  myservo.write(servo_position_down);
  server.send_P(200, "text/html", index_html);
  delay(500);
}
