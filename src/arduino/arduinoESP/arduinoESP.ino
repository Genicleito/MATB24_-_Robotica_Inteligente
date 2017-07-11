
#include <string.h>
#include "BASEROBO.h"

// enum inteiros { _tempID, _humidID, _lampID };
enum inteiros { _esqID, _dirID, _esqSpeedID, _dirSpeedID };

int direita,esquerda;
int _dirSpeed,_esqSpeed;

void setup() {
  // start the Ethernet connection:
  // Ethernet.begin(mac, ip);
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

    pinMode(DIRECAO_DIREITA_1, OUTPUT);
    pinMode(DIRECAO_DIREITA_2, OUTPUT);
    pinMode(DIRECAO_ESQUERDA_1, OUTPUT);
    pinMode(DIRECAO_ESQUERDA_2, OUTPUT);

    // // Configuração dos pinos do Encoder Ótico
    // pinMode(ENCODER_DIREITA, INPUT_PULLUP);
    // pinMode(ENCODER_ESQUERDA, INPUT_PULLUP);
    //
    // // Funções de Interrupção de cada um dos Encoders
    // attachInterrupt(digitalPinToInterrupt(ENCODER_DIREITA), contadorDireita, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(ENCODER_ESQUERDA), contadorEsquerda, CHANGE);


  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}

void messageHandler();
void loop() {
  if(Serial.available() > 0){
    messageHandler();
  }

}

void messageHandler(){
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
        case _esqID:
          esquerda = value;

          break;
        case _dirID:
          direita = value;

          break;
        case _esqSpeedID:
          // _esqSpeed = value;
          switch (esquerda) {
            case 0:
              FREIO_ESQUERDA();
              break;
            case 1:
              Serial.print("Esquerda: ");
              Serial.println(value);
              ACELERA_ESQUERDA(value);
              IR_PARA_FRENTE_ESQUERDA();
              break;
            // case -1:
            //   ACELERA_ESQUERDA(-value);
            //   break;

          }
          break;

        case _dirSpeedID:
          switch (direita) {
            case 0:
              FREIO_DIREITA();
              break;
            case 1:
              Serial.print("Direita: ");
              Serial.println(value);
              ACELERA_DIREITA(value);
              IR_PARA_FRENTE_DIREITA();
              break;
            // case -1:
            //   ACELERA_ESQUERDA(-value);
            //   break;

          }
          break;

        default:
          String aux = String(retorno);
          String resposta = "MErro: 404";
          char buffer[21];
          Serial.println(resposta);
          // serverClients[i].write(buffer, resposta.length());
          return;

      }
      break;
    case 'G':
      switch (property) {
        default:
          String aux = String(retorno);
          String resposta = "MErro: 404";
          char buffer[21];
          Serial.println(resposta);
          // serverClients[i].write(buffer, resposta.length());
          return;
      }

      String aux = String(retorno);
      String resposta = "R" + aux;
      Serial.println(resposta);
      // serverClients[i].write(buffer, resposta.length());
      break;
  }

}
