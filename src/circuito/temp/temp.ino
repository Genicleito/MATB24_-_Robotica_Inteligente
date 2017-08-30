#include <Ultrasonic.h>
#include <Servo.h>

Servo servoInferior;

//Define os pinos para o trigger e echo
#define pino_trigger 3
#define pino_echo 2

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonicFrente(pino_trigger, pino_echo);

Ultrasonic ultrasonicEsquerda(A1, A0);

Ultrasonic ultrasonicDireita(11, 10);

float distanciaEsquerda, distanciaDireita, distanciaFrente;

//#define ENCODER_DIREITA 2
//#define ENCODER_ESQUERDA 3

#define ACELERADOR_DIREITA 5
#define ACELERADOR_ESQUERDA 6

//  Definição das entradas para os motores de cada lado, observem que alguns robôs precisaram ter os pinos trocados devido aos módulos de comunicação que utilizam.
//  Vejam os 4 fios unidos da ponte H que são conectados no Arduino para saberem o padrão do robô que vocês tem em mãos.
/*#define DIRECAO_DIREITA_1 7   //4
#define DIRECAO_DIREITA_2 8   //7

#define DIRECAO_ESQUERDA_1 9  //8
#define DIRECAO_ESQUERDA_2 10 //9*/

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
int velocidadeDireita  = 195;
int velocidadeEsquerda = 175;

long microsec;

void setup() {
  // Configuração da Comunicação Serial
  Serial.begin(9600);

  // Configuração dos pinos da Ponte H
  pinMode(DIRECAO_DIREITA_1, OUTPUT);
  pinMode(DIRECAO_DIREITA_2, OUTPUT);
  pinMode(DIRECAO_ESQUERDA_1, OUTPUT);
  pinMode(DIRECAO_ESQUERDA_2, OUTPUT);

delay(2000);

  do {

      ACELERA_DIREITA(velocidadeDireita);
      ACELERA_ESQUERDA(velocidadeEsquerda);
      IR_PARA_FRENTE_DIREITA();
      IR_PARA_FRENTE_ESQUERDA();
      delay(100);
      FREIO();

      microsec = ultrasonicFrente.timing();
      distanciaFrente = ultrasonicFrente.convert(microsec, Ultrasonic::CM);
      delay(100);
      float distanciaFrente2,distanciaFrente3;

      microsec = ultrasonicFrente.timing();
      distanciaFrente2 = ultrasonicFrente.convert(microsec, Ultrasonic::CM);
      delay(100);

      microsec = ultrasonicFrente.timing();
      distanciaFrente3 = ultrasonicFrente.convert(microsec, Ultrasonic::CM);
      delay(100);

      if (distanciaFrente2 < distanciaFrente) {
          distanciaFrente = distanciaFrente2;
      }
      if (distanciaFrente3 < distanciaFrente) {
          distanciaFrente = distanciaFrente3;
      }
      /* code */
  } while(distanciaFrente > 10);

  ACELERA_DIREITA(velocidadeDireita);
  ACELERA_ESQUERDA(velocidadeEsquerda);
  IR_PARA_FRENTE_DIREITA();
  IR_PARA_TRAS_ESQUERDA();
  delay(180);

  //servoInferior.attach(10);

  // Configuração dos pinos do Encoder Ótico
//  pinMode(ENCODER_DIREITA, INPUT_PULLUP);
//  pinMode(ENCODER_ESQUERDA, INPUT_PULLUP);

  // Funções de Interrupção de cada um dos Encoders
//  attachInterrupt(digitalPinToInterrupt(ENCODER_DIREITA), contadorDireita, CHANGE);
//  attachInterrupt(digitalPinToInterrupt(ENCODER_ESQUERDA), contadorEsquerda, CHANGE);
}

void loop() {

    ACELERA_DIREITA(velocidadeDireita);
    ACELERA_ESQUERDA(velocidadeEsquerda);
    IR_PARA_FRENTE_DIREITA();
    IR_PARA_FRENTE_ESQUERDA();
    delay(100);
    FREIO();

  long microsec = ultrasonicEsquerda.timing();
  distanciaEsquerda = ultrasonicEsquerda.convert(microsec, Ultrasonic::CM);
  delay(100);

  Serial.print("distanciaFrente: ");
  Serial.println(distanciaFrente);
  Serial.print("distanciaDireita: ");
  Serial.println(distanciaDireita);
  Serial.print("distanciaEsquerda: ");
  Serial.println(distanciaEsquerda);
  Serial.println();

//   if(distanciaDireita <= 9 || ){
//     Serial.println("virando pra esquerda!");
//     ACELERA_DIREITA(velocidadeDireita + 35);
//     ACELERA_ESQUERDA(velocidadeEsquerda - 50);
//     IR_PARA_FRENTE_DIREITA();
//     IR_PARA_FRENTE_ESQUERDA();
//     delay(100);
// /*    ACELERA_DIREITA(velocidadeDireita);
//     ACELERA_ESQUERDA(velocidadeEsquerda);
//     IR_PARA_FRENTE_DIREITA();
//     IR_PARA_FRENTE_ESQUERDA();*/
//     FREIO();
//   }

    // if (contador_esquerda > 8){
    //
    //     ACELERA_DIREITA(velocidadeDireita + 25);
    //     ACELERA_ESQUERDA(velocidadeEsquerda + 25);
    //     IR_PARA_FRENTE_DIREITA();
    //     IR_PARA_FRENTE_ESQUERDA();
    //     delay(430);
    //     IR_PARA_TRAS_DIREITA();
    //     IR_PARA_TRAS_ESQUERDA();
    //     delay(460);
    //     // mete na direita
    //     ACELERA_DIREITA(velocidadeDireita - 35);
    //     ACELERA_ESQUERDA(velocidadeEsquerda + 50);
    //     IR_PARA_FRENTE_DIREITA();
    //     IR_PARA_FRENTE_ESQUERDA();
    //     delay(400);
    //
    //
    // }

  if(distanciaDireita <= 10 ){
      contador_esquerda++;
    Serial.println("virando pra esquerda!");

    ACELERA_DIREITA(velocidadeDireita);
    ACELERA_ESQUERDA(velocidadeEsquerda);
    IR_PARA_TRAS_DIREITA();
    IR_PARA_TRAS_ESQUERDA();
    delay(40);

    ACELERA_DIREITA(velocidadeDireita + 50);
    ACELERA_ESQUERDA(velocidadeEsquerda - 30);
    IR_PARA_FRENTE_DIREITA();
    IR_PARA_FRENTE_ESQUERDA();
    delay(140);
/*    ACELERA_DIREITA(velocidadeDireita);
    ACELERA_ESQUERDA(velocidadeEsquerda);
    IR_PARA_FRENTE_DIREITA();
    IR_PARA_FRENTE_ESQUERDA();*/
    FREIO();
  }

  microsec = ultrasonicDireita.timing();
  distanciaDireita = ultrasonicDireita.convert(microsec, Ultrasonic::CM);
  delay(100);

  if(distanciaEsquerda <= 10){
      contador_esquerda = 0;
    Serial.println("virando pra direita!");

    ACELERA_DIREITA(velocidadeDireita);
    ACELERA_ESQUERDA(velocidadeEsquerda);
    IR_PARA_TRAS_DIREITA();
    IR_PARA_TRAS_ESQUERDA();
    delay(40);
    ACELERA_DIREITA(velocidadeDireita - 25);
    ACELERA_ESQUERDA(velocidadeEsquerda + 70);
    IR_PARA_FRENTE_DIREITA();
    IR_PARA_FRENTE_ESQUERDA();
    delay(150);
    FREIO();
/*    ACELERA_DIREITA(velocidadeDireita);
    ACELERA_ESQUERDA(velocidadeEsquerda);
    IR_PARA_FRENTE_DIREITA();
    IR_PARA_FRENTE_ESQUERDA();*/
  }

  microsec = ultrasonicFrente.timing();
  distanciaFrente = ultrasonicFrente.convert(microsec, Ultrasonic::CM);
  delay(100);
  float distanciaFrente2,distanciaFrente3;

  microsec = ultrasonicFrente.timing();
  distanciaFrente2 = ultrasonicFrente.convert(microsec, Ultrasonic::CM);
  delay(100);

  microsec = ultrasonicFrente.timing();
  distanciaFrente3 = ultrasonicFrente.convert(microsec, Ultrasonic::CM);
  delay(100);

  if (distanciaFrente2 < distanciaFrente) {
      distanciaFrente = distanciaFrente2;
  }
  if (distanciaFrente3 < distanciaFrente) {
      distanciaFrente = distanciaFrente3;
  }


  if(distanciaFrente <= 18){

    FREIO();
    delay(100);

    microsec = ultrasonicEsquerda.timing();
    distanciaEsquerda = ultrasonicEsquerda.convert(microsec, Ultrasonic::CM);
    delay(100);
    microsec = ultrasonicDireita.timing();
    distanciaDireita = ultrasonicDireita.convert(microsec, Ultrasonic::CM);
    delay(100);

    ACELERA_DIREITA(velocidadeDireita);
    ACELERA_ESQUERDA(velocidadeEsquerda);
    IR_PARA_TRAS_DIREITA();
    IR_PARA_TRAS_ESQUERDA();
    delay(40);

    if(distanciaEsquerda >= distanciaDireita){
    //   ACELERA_DIREITA(velocidadeDireita);
    //   ACELERA_ESQUERDA(velocidadeEsquerda);
    //   IR_PARA_FRENTE_DIREITA();
    //   IR_PARA_TRAS_ESQUERDA();
    //   delay(140);}
    /*      ACELERA_DIREITA(velocidadeDireita);
      ACELERA_ESQUERDA(velocidadeEsquerda);
      IR_PARA_FRENTE_DIREITA();
      IR_PARA_FRENTE_ESQUERDA();*/
    }else{
      ACELERA_DIREITA(velocidadeDireita);
      ACELERA_ESQUERDA(velocidadeEsquerda);
      IR_PARA_TRAS_DIREITA();
      IR_PARA_FRENTE_ESQUERDA();
      delay(140);
    /*      ACELERA_DIREITA(velocidadeDireita);
      ACELERA_ESQUERDA(velocidadeEsquerda);
      IR_PARA_FRENTE_DIREITA();
      IR_PARA_FRENTE_ESQUERDA();*/
    }
    FREIO();
  }
}

void contadorDireita(){
  contador_direita++;
}

void contadorEsquerda(){
  contador_esquerda++;
}
