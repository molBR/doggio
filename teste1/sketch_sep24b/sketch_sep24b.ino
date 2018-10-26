#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
 
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>
int switchReset =16;
unsigned long intervalo = 5000;
long currentMillis = 0;
long previousMillis = 0;
bool aux = 0;
 
void setup() {
    pinMode(switchReset,INPUT);
    // put your setup code here, to run once:
    Serial.begin(115200);
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
 
    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();
 
    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

}
void clearEeprom(){
  WiFi.disconnect();
  EEPROM.begin(4096);
  Serial.println("CLEANING");
  delay(1000);
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, '0');
  }
  EEPROM.commit();
  ESP.reset();  
  
}
void loop() {
  int valor = 0;
 
  valor = digitalRead(switchReset);
  if(!valor){
    if(!aux){
     previousMillis = millis();
    }
    aux = 1;
  }
  if(aux){
    currentMillis = millis();
    Serial.println(currentMillis - previousMillis);
    if(!valor && (currentMillis - previousMillis >= intervalo)){
      Serial.println("cleaning");
      clearEeprom();
    }
  }if(aux && valor){
    aux = 0;
  }
} 
