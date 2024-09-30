#include <Arduino.h>
#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<DNSServer.h>
#include<WiFiManager.h>
#include<ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include<string.h>

WiFiClient client;
HTTPClient http;
WiFiManager wifi;

String URL;
String API ="5D2A8K4S7NBAH251";
String fieldNo = "1";
String response,result;
int begin_string,end_string; 
int httpcode;

void Connect_to_WiFi_StationMode(String,String);
void send_data(int);
String get_data_from_channel_1(String,String);
int ADC(void);
void POSTaddCommand(String command);
void PUTupdateCommand(String command_id, String command);
String GETCommand(String command_id);
void EXECUTECommand(void);

void Connect_to_WiFi_StationMode(String SSID, String password)
{
   WiFi.mode(WIFI_STA);
   WiFi.begin(SSID,password);
   Serial.print("\n\nconnecting to wifi");
     while(WiFi.status() != WL_CONNECTED)
      {
        Serial.print(".");
        delay(200);
      }
   Serial.print("\nIP address : ");
   Serial.print(WiFi.localIP());
   Serial.print("\nMAC address");
    Serial.print(WiFi.macAddress());
}

void send_data(int data)
{
  //https://api.thingspeak.com/update?api_key=5D2A8K4S7NBAH251&field1=3
   URL = "http://api.thingspeak.com/update?api_key=";
   URL = URL + API;
   URL = URL + "&field";
   URL = URL + fieldNo;
   URL = URL + "=";
   URL = URL + data;

   http.begin(client,URL);
   Serial.println("\nwaiting for response(DATA being SENT)");
   httpcode = http.GET();
    if(httpcode > 0)
    {
      Serial.println("data sent successfully");
    }

    else
    {
      Serial.println("Error in sending DATA");
    }

   http.end();
   delay(16000);
}

int ADC()
{
  int input_analog = analogRead(A0);
  delay(200);
  Serial.print("Pot. Value : ");
  Serial.print(input_analog);
  Serial.print("\n");

  return input_analog;
}

String get_data_from_channel_1(String Field_num,String channel_id)
{

  URL = "http://api.thingspeak.com/channels/";//2360280/fields/";
  URL = URL + channel_id;
  URL = URL + "/fields/";
  URL = URL + Field_num;
  URL = URL + ".json?api_key=Q0U1MSNGUUVEH1AE&results=1";

  http.begin(client,URL);
  Serial.print("Waiting for response(GET)\n");
  httpcode = http.GET();
  if(httpcode > 0)
  {
    Serial.println("GET req sent ");
    response = http.getString();
   // Serial.println(response);
    Serial.print("\n");
    end_string   = response.indexOf("}]}");
    begin_string = response.indexOf("\"entry_id");
    result       = response.substring(begin_string+23,end_string-1);
    return result;
  }
  else
  {
    Serial.println("failed to connect to server");
  }
  http.end();
  return (" "); 

}

void POSTaddCommand(String command)
{
  //http://api.thingspeak.com/talkbacks/50750/commands.json?api_key=ONH87T3ICE4RB60U&command_string=L1
  http.begin(client,"http://api.thingspeak.com/talkbacks/50750/commands");
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  String postMessage = "api_key=ONH87T3ICE4RB60U&command_string=";
  postMessage     = postMessage + command;
  postMessage     = postMessage + "\r\n\r\n";
  int httpcode    = http.POST(postMessage);
  String payload  = http.getString();
  Serial.println(httpcode);
  if(httpcode > 0)
  {
    Serial.println(payload);
  }
  else
  {
    Serial.println("HTTP error");
  }
  http.end();
  delay(1000);
}

void PUTupdateCommand(String command_id, String command)
{
  //http://api.thingspeak.com/talkbacks/50750/commands/38175641.json?api_key=ONH87T3ICE4RB60U&command_string=L4
  URL = "http://api.thingspeak.com/talkbacks/50750/commands/";
  URL = URL + command_id;
  URL = URL + ".json?api_key=4300H2OYN8EMPFWC&command_string=";
  URL = URL + command;
  http.begin(client,URL);
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  String putMessage = "api_key=4300H2OYN8EMPFWC&command_string=";
  putMessage     = putMessage + command;
  putMessage     = putMessage + "\r\n\r\n";
  int httpcode    = http.PUT(putMessage);
  String payload  = http.getString();
  if(httpcode > 0)
  {
    Serial.println(payload);
  }
  else
  {
    Serial.println("HTTP error");
  }
  http.end();
  delay(1000);
}

String GETCommand(String command_id)
{
  //https:api.thingspeak.com/talkbacks/50851/commands/COMMAND_ID.json?api_key=4300H2OYN8EMPFWC
  URL = "http://api.thingspeak.com/talkbacks/50851/commands/";
  URL = URL + command_id;
  URL = URL + ".json?api_key=4300H2OYN8EMPFWC";
  http.begin(client,URL);
  //Serial.print("Waiting for response(GET)\n");
  httpcode = http.GET();
  int start_index = response.indexOf("command_string\":\"")+17;
  result = response.substring(start_index,start_index+2);
  if(httpcode > 0)
  {
    response = http.getString();
    return result;
  }
  else
  {
    Serial.println("HTTP error");
  }
  http.end();
  return (" ");
}

void EXECUTECommand()
{
  http.begin(client,"http://api.thingspeak.com/talkbacks/50750/commands/execute");
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  String postMessage = "api_key=ONH87T3ICE4RB60U&command_string=";
  postMessage     = postMessage + "\r\n\r\n";
  int httpcode    = http.POST(postMessage);
  String payload  = http.getString();
  if(httpcode > 0)
  {
    Serial.println(payload);
  }
  else
  {
    Serial.println("HTTP error");
  }
  http.end();
  delay(1000);
}