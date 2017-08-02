/*
 * Raio da roda do robô é 3,25 cm
 * A uma aceleração de 255, o robô dar uma volta completa (equivalente a ~20,41cm) na roda direita em 600 ms
*/
#define ENCODER_DIREITA 2
#define ENCODER_ESQUERDA 3

#define ACELERADOR_DIREITA 5
#define ACELERADOR_ESQUERDA 6

//  Definição das entradas para os motores de cada lado, observem que alguns robôs precisaram ter os pinos trocados devido aos módulos de comunicação que utilizam.
//  Vejam os 4 fios unidos da ponte H que são conectados no Arduino para saberem o padrão do robô que vocês tem em mãos.
#define DIRECAO_DIREITA_1 9   //4
#define DIRECAO_DIREITA_2 8   //7

#define DIRECAO_ESQUERDA_1 7  //8
#define DIRECAO_ESQUERDA_2 4 //9

#define IR_PARA_FRENTE_DIREITA() do { digitalWrite(DIRECAO_DIREITA_1, HIGH); digitalWrite(DIRECAO_DIREITA_2, LOW); } while(false)
#define IR_PARA_FRENTE_ESQUERDA() do { digitalWrite(DIRECAO_ESQUERDA_1, HIGH); digitalWrite(DIRECAO_ESQUERDA_2, LOW); } while(false)
#define IR_PARA_FRENTE() do { IR_PARA_FRENTE_DIREITA(); IR_PARA_FRENTE_ESQUERDA(); } while(false)

#define IR_PARA_TRAS_DIREITA() do { digitalWrite(DIRECAO_DIREITA_1, LOW); digitalWrite(DIRECAO_DIREITA_2, HIGH); } while(false)
#define IR_PARA_TRAS_ESQUERDA() do { digitalWrite(DIRECAO_ESQUERDA_1, LOW); digitalWrite(DIRECAO_ESQUERDA_2, HIGH); } while(false)
#define IR_PARA_TRAS() do { IR_PARA_TRAS_DIREITA(); IR_PARA_TRAS_ESQUERDA(); } while(false)

#define ACELERA_DIREITA(VELOCIDADE) do { pwmDireita = VELOCIDADE; analogWrite(ACELERADOR_DIREITA, VELOCIDADE); } while(false)
#define ACELERA_ESQUERDA(VELOCIDADE) do { pwmEsquerda = VELOCIDADE; analogWrite(ACELERADOR_ESQUERDA, VELOCIDADE); } while(false)
#define ACELERA(VELOCIDADE) do { ACELERA_DIREITA(VELOCIDADE); ACELERA_ESQUERDA(VELOCIDADE); } while(false)

#define FREIO_DIREITA() { ACELERA_DIREITA(0); digitalWrite(DIRECAO_DIREITA_1, LOW); digitalWrite(DIRECAO_DIREITA_2, LOW); } while(false)
#define FREIO_ESQUERDA() { ACELERA_ESQUERDA(0); digitalWrite(DIRECAO_ESQUERDA_1, LOW); digitalWrite(DIRECAO_ESQUERDA_2, LOW); } while(false)
#define FREIO() do { FREIO_DIREITA(); FREIO_ESQUERDA(); } while(false)

// Velocidade armazenada PWM
volatile int pwmDireita = 0;
volatile int pwmEsquerda = 0;

// Numero de passos do Encoder Ótico
volatile int contador_direita = 0;
volatile int contador_esquerda = 0;

// Definir velocidades
int velocidadeDireita  = 255;
int velocidadeEsquerda = 230;

int neighbours[] = {0,3,6,7,8,5,2,1};
int corners[] = {0,2,6,8};
int Ts[] = {1,3,5,7};
int centro = 4;

bool pattern [3][3] = {         {0,0,0},
                                {0,0,0},
                                {0,0,0}
    };
bool patternStraight [3][3] = { {0,1,0},
                                {0,1,0},
                                {0,1,0}
    };

bool patternT [3][3] = {        {0,1,0},
                                {1,1,0},
                                {0,1,0}
    };

bool patternCorner [3][3] = {   {0,0,0},
                                {1,1,0},
                                {0,1,0}
    };

bool patternCenter [3][3] = {   {0,1,0},
                                {1,1,1},
                                {0,1,0}
    };


void setup() {
  // Configuração da Comunicação Serial
  Serial.begin(115200);

  // Configuração dos pinos da Ponte H
  pinMode(DIRECAO_DIREITA_1, OUTPUT);
  pinMode(DIRECAO_DIREITA_2, OUTPUT);
  pinMode(DIRECAO_ESQUERDA_1, OUTPUT);
  pinMode(DIRECAO_ESQUERDA_2, OUTPUT);

  // Configuração dos pinos do Encoder Ótico
  pinMode(ENCODER_DIREITA, INPUT_PULLUP);
  pinMode(ENCODER_ESQUERDA, INPUT_PULLUP);

  // Funções de Interrupção de cada um dos Encoders
  attachInterrupt(digitalPinToInterrupt(ENCODER_DIREITA), contadorDireita, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_ESQUERDA), contadorEsquerda, CHANGE);
}



void loop() {
  float frente = analogRead(A0), tras = analogRead(A1), centro = analogRead(A2), direita = analogRead(A3), esquerda = analogRead(A4);


  // if(analogRead(A0) > 200){ // Frente esta na linha
  while(checkPattern()){ // Frente esta na linha
    ACELERA_DIREITA(velocidadeDireita);
    ACELERA_ESQUERDA(velocidadeEsquerda);
    IR_PARA_FRENTE();
    delay(70);
    FREIO();
    delay(500);
  }


}

#define FRENTE()    analogRead(A0)
#define TRAS()      analogRead(A1)
#define CENTRO()    analogRead(A2)
#define DIREITA()   analogRead(A3)
#define ESQUERDA()  analogRead(A4)

int current = 0;
bool checkPattern(){

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

        }
    }

    return true;

}



void contadorDireita(){
  contador_direita++;
}

void contadorEsquerda(){
  contador_esquerda++;
}
