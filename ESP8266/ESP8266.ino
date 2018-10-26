#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
//192.168.4.1
bool Aux = false;
//WiFiServer server(80);
ESP8266WebServer server(80);
int switchReset =16;
void paginaSimples(){
  String buf = "";
  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n";
  buf += "<html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>\r\n";
  buf += "<title>ESP8266 Web Server</title>";
  buf += "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:80%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;}</style>";
  buf += "</head>";
  buf += "<h3>ESP8266 Coloque as credenciais do wifi de sua residencia</h3>";
  buf += "<form method = 'POST' action='/form'>";
  buf += "<p>WiFi</p> <input type=text name=txtSSID><br>";
  buf += "<p>Senha</p> <input type=password name=txtSenha><br>";
  buf += "<input type = submit name=botao value=Enviar />";
  buf += "</form>";
  buf += "<h4>Criado por Pedro Vallese</h4>";
  buf += "</html>\n";

  server.send(200, "text/html", buf);
}

bool eepromNotEmpty(){
  EEPROM.begin(4096);
  Serial.println("DATA:");
  bool checkAux = false;
  char charAux;
  for (int i = 0;i<EEPROM.length();i++){
    charAux = (char)EEPROM.read(i);
    Serial.println(charAux);
    checkAux = false;
    if(charAux =='0')
    {
      Serial.println("EEPROM VAZIA");
      checkAux = true;
    }
    if(checkAux){
      return false;
    }
  }
  if(checkAux==false){
    Serial.println("EEPROM NAO VAZIA");
    return true; 
  }
}
void clearEeprom(){
  EEPROM.begin(4096);
  Serial.println("CLEANING");
  delay(1000);
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, '0');
  }
  EEPROM.commit();
}

void setup() {
  
  Serial.begin(115200);
  pinMode(switchReset,INPUT);
  if(eepromNotEmpty()){
    char login[4096];
    char senha[4096];
    EEPROM.begin(4096);
    int auxRead=0;
    int auxWrite=0;
    for(int i =0; i<EEPROM.length();i++){
      if(EEPROM.read(i)=='\0') {
        auxRead++;
        auxWrite = 0;
        i++;
      }
      if(auxRead==0) {
        login[auxWrite] = EEPROM.read(i);
        auxWrite++;
      }
      else if(auxRead==1){
        senha[auxWrite] = EEPROM.read(i);
        auxWrite++;
      }
      else {
        break;
      }
    }
    
    Serial.println("CREDENCIAIS");
    Serial.println(login);
    Serial.println(senha);
    Serial.println("FIM CREDENCIAIS");
    WiFi.disconnect();
    WiFi.begin(login, senha);
    Serial.println(";");
    delay(5000);
    int checkCon =0;
    bool failCon =false;
    while (WiFi.status() != WL_CONNECTED && checkCon<50) {
      delay(500);
      Serial.print(".");
      checkCon++;
      if(checkCon == 49){
        Serial.println("Connection Failed!");
        failCon = true;
      }
    }
    if(!failCon){
      Serial.println("Connected!");
      
    }
  }else{
    Serial.print("Setting soft-AP ... ");
    boolean result = WiFi.softAP("WifiComedia", "atireiopaunogato");
    if(result == true)
    {
      Serial.println("Ready");
    }
    else
    {
      Serial.println("Failed!");
    }   
    WiFi.mode(WIFI_AP);
    
    server.on("/", paginaSimples);
    server.on("/form", HTTP_POST, formularioEnviado);
    server.begin();
    Serial.println("Server started");
    Serial.println(WiFi.localIP());
  }
  
}

void formularioEnviado(){
  String ssid, senha;
  char c,leitor='1';
  int i,j;
  EEPROM.begin(4096);
  if(server.hasArg("txtSSID")){
    Serial.print("SSID:");
    ssid = server.arg("txtSSID");
    Serial.println(ssid);
  }else{
    Serial.println("Vazio");
  }
  if(server.hasArg("txtSenha")){
    Serial.print("Senha:");
    senha = server.arg("txtSenha");
    Serial.println(senha);
  }
  clearEeprom();
  for (i=0;i<ssid.length();i++){
    c = ssid[i];
    EEPROM.write(i,c);
  }
    //i++;
    EEPROM.write(i,'\0');
    EEPROM.commit();
 //   EEPROM.end();
    i++;
    int lastSpace = i;
    
   // EEPROM.begin(sizeof(senha)+1);
     for (i=0;i<senha.length();i++){

      c = senha[i];
      EEPROM.write(lastSpace + i,c);
    }
    lastSpace = lastSpace + i;
    EEPROM.write(lastSpace,'\0');
    EEPROM.commit();
    j=0;
    while(leitor!='\0'){
      leitor = (char)EEPROM.read(j);
      Serial.println(leitor);
      j++;
    }
    leitor='a';
    while(leitor!='\0'){
      leitor = (char)EEPROM.read(j);
      Serial.println(leitor);
      j++;
    }

  EEPROM.end();
}

 
void loop() {
  int valor = 0;
  valor = digitalRead(switchReset);
  if(valor >0){
    clearEeprom();
    Serial.println("EEPROM CLEAR...RESTART");
    ESP.restart();
  }

 delay(10000);
 server.handleClient();
}
 
 
 

