#include <SPI.h>
#include <ESP8266WiFi.h>
#include <string.h>

#define MAX_SRV_CLIENTS 1
const char* ssid = "**********";
const char* password = "**********";

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

// enum inteiros { _tempID, _humidID, _lampID };
enum inteiros { _tempID, _humidID };
int _temp = 32
    , _humid = 60
    , _lamp = 1 ;
#define _lampID 12

#define client Serial

void setup() {
  // start the Ethernet connection:
  // Ethernet.begin(mac, ip);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial1.print("Could not connect to"); Serial1.println(ssid);
    while(1) delay(500);
  }

  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("Waiting Message");

  server.begin();
  server.setNoDelay(true);
  Serial1.print("Ready! Use 'telnet ");
  Serial1.print(WiFi.localIP());
  Serial1.println(" 23' to connect");

}

void loop() {
  uint8_t i,j;
  //check if there are any new clients
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial1.print("New client: "); Serial1.print(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  //check clients for data
  char buffer[21];
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){

        for (j = 0; serverClients[i].available(); j++) {
          buffer[i] = serverClients[i].read();
        }
        String message(buffer);
        messageHandler(message,i);
      }
    }
  }
}

void messageHandler(String message,int i){
  String message;
  int retorno;

  message = Serial.readStringUntil('\n');
  // Serial.println(message);//<debug>
  int property = atoi(&message[1]);
  switch (message[0]) {
    case 'S':
      int value;
      value = atoi(&message[message.indexOf('=')+1]);
      switch (property) {
        case _tempID:
          _temp = value;
          break;
        case _humidID:
          _humid = value;
          break;
        case _lampID:
          _lamp = value;
          break;
      }
      break;
    case 'G':
      switch (property) {
        case _tempID:
          retorno = _temp;
          break;
        case _humidID:
          retorno = _humid;
          break;
        case _lampID:
          retorno = _lamp;
          break;
        default:
          String aux = String(retorno);
          String resposta = "MErro: 404";
          char buffer[21];
          string.toCharArray(buffer, len)
          serverClients[i].write(buffer, resposta.length());
          return;
      }
      String aux = String(retorno);
      String resposta = "R" + aux;
      char buffer[21];
      string.toCharArray(buffer, len)
      serverClients[i].write(buffer, resposta.length());
      break;
  }

}
