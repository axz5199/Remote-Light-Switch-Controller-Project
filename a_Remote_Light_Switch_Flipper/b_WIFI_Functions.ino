void handleRoot()
{
  server.send_P(200, "text/html", index_html);
}

void handleUp()
{
  Serial.println("Up pressed");
  myservo.write(120);
  server.send_P(200, "text/html", index_html);
  delay(500);
}

void handleDown()
{
  Serial.println("Down pressed");
  myservo.write(45);
  server.send_P(200, "text/html", index_html);
  delay(500);
}
