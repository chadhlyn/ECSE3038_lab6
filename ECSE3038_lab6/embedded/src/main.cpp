#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

const int ledPinNum1 = 25;
const int ledPinNum2 = 26;
const int ledPinNum3 = 27;


bool led_sequence [8][3]= { {false, false, false},        
                            {false, false, true},
                            {false, true,  false},
                            {false, true,  true},
                            {true,  false, false},
                            {true,  false, true},
                            {true,  true,  false},
                            {true,  true,  true},
                           };


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPinNum1, OUTPUT);
  pinMode(ledPinNum2, OUTPUT);
  pinMode(ledPinNum3, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASS); //these should be stored in secret.h and .getignore to hide from public
  
  Serial.println("Connecting");
  while (WiFi.status()!= WL_CONNECTED){
    delay (500);
    Serial.print(".");
  }
  Serial.print("");
  Serial.print("connected to WiFi network with IP Address: ");
  Serial.println (WiFi.localIP());
}



void loop() {
  //this check for the internet conectivity status
  if (WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    String http_response;
    http.begin(host);                               //starts the connection to the server
    
    for (int i = 0; i < 8; i++)
    {

    http.addHeader("Content-Type","application/json"); 
    http.addHeader("X-API-Key", API_KEY);

    StaticJsonDocument<76> doc;                      //create so we can put thing in this
    doc["light_switch_1"] = led_sequence[i][0];
    doc["light_switch_2"] = led_sequence[i][1];
    doc["light_switch_3"] = led_sequence[i][2];

    digitalWrite(ledPinNum1, led_sequence[i][0]);
    digitalWrite(ledPinNum2, led_sequence[i][1]);
    digitalWrite(ledPinNum3, led_sequence[i][2]);


    String httpRequest; 
    serializeJson(doc,httpRequest);
    int httpResponseCode = http.PUT(httpRequest);     //This is to do a get request
                                                      //this code return an integer (status code)
                                      
    if (httpResponseCode > 0){
      Serial.print("HTTP Response Code:  ");
      Serial.println(httpResponseCode);

      Serial.print("Response from server: ");
      http_response = http.getString(); //this is getting a response from the server
      Serial.println(http_response);
    }
    else {
      Serial.print("Error code:  ");
      Serial.print(httpResponseCode);
    }
    delay(2000);
    }
    
    
    //free resourses
    http.end();
  }
  else{
    return;
  }
}