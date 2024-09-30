#include <Arduino.h>
#include<ESP8266WiFi.h>
#include"ESP_ThingSpk_init.h"
 String command_str;
void setup()
{
  Serial.begin(9600);
  Connect_to_WiFi_StationMode("kavi","12345678");
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop()
{
  command_str =  GETCommand("38712960");
  if(command_str == "L1")
  {
    digitalWrite(LED_BUILTIN,LOW);
  }
  if(command_str == "L2")
  {
    digitalWrite(LED_BUILTIN,HIGH);
  }
  Serial.println(command_str);
  
  //int value = ADC();
  //send_data(value);
}
