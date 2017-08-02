
#include <string.h>
#include "BASEROBO.h"

#define centro 4

// enum inteiros { _tempID, _humidID, _lampID };
enum inteiros { _esqID, _dirID, _esqSpeedID, _dirSpeedID, _posID };

int direita,esquerda;
int _dirSpeed,_esqSpeed;

int neighbours[] = {0,3,6,7,8,5,2,1};
int corners[] = {0,2,6,8};
int current = 0;



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
          Serial.print("Seta esquerda: ");
          Serial.print(value);
          break;
        case _dirID:
          direita = value;
          Serial.print("Seta direita: ");
          Serial.print(value);
          break;

        case _esqSpeedID:
          // _esqSpeed = value;
          switch (esquerda) {
            case 0:
              FREIO_ESQUERDA();
              break;
            case 1:
              Serial.print("Esquerda Frente: ");
              Serial.print(value);
              ACELERA_ESQUERDA(value);
              IR_PARA_FRENTE_ESQUERDA();
              break;
            case -1:
              Serial.print("Esquerda: Tras");
              Serial.print(value);
              ACELERA_ESQUERDA(value);
              IR_PARA_TRAS_ESQUERDA();
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
              Serial.print("Direita Frente: ");
              Serial.println(value);
              ACELERA_DIREITA(value);
              IR_PARA_FRENTE_DIREITA();
              break;
            case -1:
              Serial.print("Direita Tras: ");
              Serial.println(value);
              ACELERA_DIREITA(value);
              IR_PARA_TRAS_DIREITA();
              break;

          }
          break;

        case _posID:
          //----
          moveTo(value);
          String aux = String(retorno);
          String resposta = "End";
          char buffer[21];
          Serial.println(resposta);

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

void moveTo(int move){
  while (!nextPos(move));
}

bool nextPos(int move){

    Serial.print("Read FRENTE: ");
    Serial.println(FRENTE());
    Serial.print("Read TRAS: ");
    Serial.println(TRAS());
    Serial.print("Read CENTRO: ");
    Serial.println(CENTRO());
    Serial.print("Read DIREITA: ");
    Serial.println(DIREITA());
    Serial.print("Read ESQUERDA: ");
    Serial.println(ESQUERDA());
    Serial.println();

    if(FRENTE() > 500 ){
        pattern[0][1] = true;
    } else pattern[0][1] = false;
    if(TRAS() > 500 ){
        pattern[2][1] = true;
    } else pattern[2][1] = false;
    if(CENTRO() > 500 ){
        pattern[1][1] = true;
    } else pattern[1][1] = false;
    if(DIREITA() > 500 ){
        pattern[1][2] = true;
    } else pattern[1][2] = false;
    if(ESQUERDA() > 500 ){
        pattern[1][0] = true;
    } else pattern[1][0] = false;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            Serial.print(pattern[i][j]);
            Serial.print(" ");
        }

        Serial.println();
    }

    if (ESQUERDA() > 500){
        if(TRAS() > 500 && CENTRO() > 500){
            current = (current + 1) % 9;
            Serial.println("Achou!");
            if (neighbours[current] == move) return true;
        }
    }

    return false;

}

void movement(){
  size_t i;
  for (i = 0; i < 4
                && corners[i] != neighbours[current];
                    i++);
  if (i == 4){
    // Go Forward
    ACELERA_DIREITA(velocidadeDireita);
    ACELERA_ESQUERDA(velocidadeEsquerda);
    IR_PARA_FRENTE();
    delay(70);
    FREIO();
    delay(500);
  } else{
    // Turn 90º
  }

}
