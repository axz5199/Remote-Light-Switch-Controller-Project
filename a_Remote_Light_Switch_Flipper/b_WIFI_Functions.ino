void handleRoot()
{
  server.send_P(200, "text/html", index_html);
}

void handleUp()
{
  digitalWrite(pin_led, LOW);
  Serial.println("Up pressed");
  myservo.write(120);
  server.send_P(200, "text/html", index_html);
  delay(500);
}

void handleDown()
{
  digitalWrite(pin_led, HIGH);
  Serial.println("Down pressed");
  myservo.write(45);
  server.send_P(200, "text/html", index_html);
  delay(500);
}
