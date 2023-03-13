#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h" 

#define endpoint "cl-lab6-api.onrender.com"

// const int ledPinNum1 = 25;
// const int ledPinNum2 = 26;
// const int ledPinNum3 = 27;

#define fanPin 22
#define lightPin 16 

// bool led_sequence [8][3]= { {false, false, false},        
//                             {false, false, true},
//                             {false, true,  false},
//                             {false, true,  true},
//                             {true,  false, false},
//                             {true,  false, true},
//                             {true,  true,  false},
//                             {true,  true,  true},
//                            };

float getTemp(){

  return random(22.1,33.1);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // pinMode(ledPinNum1, OUTPUT);
  // pinMode(ledPinNum2, OUTPUT);
  // pinMode(ledPinNum3, OUTPUT);
  pinMode(fanPin,OUTPUT);
  pinMode(lightPin,OUTPUT);

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
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    HTTPClient http;

   // Establish a connection to the server
    String url = "https://" + String(endpoint) + "/api/state";
    http.begin(url);
    http.addHeader("Content-type", "application/json");
    http.addHeader("Content-length", "23");
    // http.addHeader("X-API-Key", API_KEY);
    
    for (int i = 0; i < 8; i++)
    {

   

    StaticJsonDocument<23> docp;                      //create so we can put thing in this
    String httpRequestData;
    // doc["temperature"] = getTemp();
    // doc["light_switch_1"] = led_sequence[i][0];
    // doc["light_switch_2"] = led_sequence[i][1];
    // doc["light_switch_3"] = led_sequence[i][2];

    docp["temperature"] = getTemp();

    // digitalWrite(ledPinNum1, led_sequence[i][0]);
    // digitalWrite(ledPinNum2, led_sequence[i][1]);
    // digitalWrite(ledPinNum3, led_sequence[i][2]);


    String httpRequest; 
    serializeJson(docp,httpRequest);
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
    delay(50);
    }
    
    
    //free resourses
    http.end();    
    http.begin(url);
    httpResponseCode = http.GET();

    Serial.println("");
    Serial.println("");

    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        Serial.print("Response from server: ");
        http_response = http.getString();
        Serial.println(http_response);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
 
    StaticJsonDocument<1024> docg;

    DeserializationError error = deserializeJson(docg, http_response);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
    
    bool temp = docg["fan"]; 
    bool light= docg["light"]; 

    digitalWrite(fanPin,temp);
    digitalWrite(lightPin,temp);
    
    // Free resources
    http.end();
  }
  else{
     Serial.println("WiFi Disconnected");
  }
}