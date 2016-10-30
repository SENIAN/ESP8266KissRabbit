#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h> 
#include <ESP8266WebServer.h>    
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MQTTClient.h>
  
  WiFiServer server(8080);
  WiFiClient espClient;
  PubSubClient client(espClient);
  WiFiClientSecure sslKisser;
  MQTTClient client1;
  
  int value = 0;

  const char *mqtt_server = NULL;
  const char *mqtt_port = NULL;
  const char *mqttuser = NULL;
  const char *mqttpassword = NULL;
  const char *mqttserver = NULL;
  const char *mqttport = NULL;
  const long interval = 2000; 
  bool shouldSaveConfig = false;
  
  int ledPin1 = 16 ; // D0
  int ledPin2 = 5 ; //D1
  int ledPin3 = 4 ; //D2
  int pin4 = 0; //D3
  
  int buttonState = 0;  
  int trillModule = 0;
  int resetPin = 0;
  int buttonPin =0;
  
  char message_buff[100];
  char message_buff1[100];
  unsigned long previousMillis = 0;
  String messagePayload;
  
  void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  Serial.println("Setting UP wifiManager");
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setConfigPortalTimeout(2000);
  wifiManager.autoConnect("KissRabit", "KissrabitModule!");
  Serial.println("Connection to MQTT server");
  if(WiFi.status() == WL_CONNECTED) {
     Serial.print("Wifi Connected! Setting up MQTT connection");
     client1.begin("m21.cloudmqtt.com", 27193, sslKisser);
     client1.connect("KissRabitSSL", "KissRabitInstance", "MD5 Hashed Password");
     client1.subscribe("KissRabbitReceiver");
     client1.publish("KissRabbitReceiver", "KissRabit Over SSL");
     Serial.print("CONNECTED!");
  }
  //registerServer();   
  registerAllPins(); 
}


void buttonPressed() {
  while (!client1.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client1.connect("KissRabit01", "KissRabitInstance", "MD5 Hashed Password")) {
      Serial.println("connected");
      client1.publish("KissRabbitReceiver", "BlowKisserSSL");
      client1.subscribe("KissRabbitReceiver");
    } else {
      Serial.print("failed, rc=");
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client1.connected()) {
    Serial.print("Re-Attempting MQTT connection...");
    if (client1.connect("KissRabit01", "KissRabitInstance", "MD5Hashed password")) {
      Serial.println("connected");
      client1.publish("KissRabbitReceiver", "BlowKisserSSL");
      client1.subscribe("KissRabbitReceiver");
    } else {
      Serial.print("failed, rc=");
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


  void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  }
  void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
 }

void registerAllPins() {
 //Register pins
 delay(10);
 pinMode(ledPin1, OUTPUT);/
 pinMode(ledPin2, OUTPUT);
 pinMode(ledPin3, OUTPUT);
 pinMode(resetPin, INPUT);
 pinMode(buttonPin, INPUT);
 pinMode(trillModule, OUTPUT);
    
 digitalWrite(ledPin1, LOW);
 digitalWrite(ledPin2, LOW);
 digitalWrite(ledPin3, LOW);
 digitalWrite(trillModule, LOW);
 digitalWrite(resetPin, LOW);
 digitalWrite(buttonPin, LOW);

}



void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.println("  " + payload); 
  messagePayload = payload; 
}


void loop() {
  client1.loop();
   if(messagePayload.equals("BlowKisser2")) {
     digitalWrite(ledPin1, LOW);
     digitalWrite(ledPin2, LOW);
     digitalWrite(ledPin3, LOW);
     digitalWrite(trillModule,LOW);
     delay(1000);
     digitalWrite(ledPin1, LOW);
     digitalWrite(trillModule,LOW);
     digitalWrite(ledPin2, LOW);
     digitalWrite(ledPin3, HIGH);
     delay(2000);    
    }
     digitalWrite(ledPin1, LOW);
     digitalWrite(ledPin2, LOW);
     digitalWrite(ledPin3, LOW);
     digitalWrite(trillModule,LOW); 
  
}
