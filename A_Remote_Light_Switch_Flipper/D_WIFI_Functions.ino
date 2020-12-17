void handleRoot()                                //When "ip_address" is entered in the address bar
{
  server.send_P(200, "text/html", index_html);   //Send response code and html to webpage
}

void handleUp()                                  //When "ip_address/up" is entered in the address bar
{
  Serial.println("Wifi up");
  myservo.attach(servo_pin);                     //Attach servo motor
  myservo.write(servo_position_up);              //Move arm up
  server.send_P(200, "text/html", index_html);   //Send response code and html to webpage
  delay(500);                                    //Provides sufficient time for arm to move
  myservo.write(servo_position_mid);
  delay(500);                                    //Provides sufficient time for arm to move
  myservo.detach();                              //Detach servo motor to conserve power
}

void handleDown()                                //When "ip_address/down" is entered in the address bar
{
  Serial.println("Wifi down");
  myservo.attach(servo_pin);                     //Attach servo motor
  myservo.write(servo_position_down);            //Move arm down
  server.send_P(200, "text/html", index_html);   //Send response code and html to webpage
  delay(500);                                    //Provides sufficient time for arm to move
  myservo.write(servo_position_mid);
  delay(500);                                    //Provides sufficient time for arm to move
  myservo.detach();                              //Detach servo motor to conserve power
}
