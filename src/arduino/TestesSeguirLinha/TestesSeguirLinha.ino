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
int velocidadeEsquerda = 255;

void setup() {
  // Configuração da Comunicação Serial
  Serial.begin(9600);

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

//  if(analogRead(A0) > 200){ // Frente esta na linha
    ACELERA_DIREITA(velocidadeDireita);
    ACELERA_ESQUERDA(velocidadeEsquerda);
    IR_PARA_FRENTE();
    delay(30);
    FREIO();
    delay(30);
//  }
  
  while (analogRead(A0) < 200){  //  Frente saiu da linha
      //FREIO();
      if(analogRead(A3) > 200){ // Direita entrou na linha
        ACELERA_DIREITA(velocidadeDireita);
        IR_PARA_TRAS_DIREITA();
        ACELERA_ESQUERDA(velocidadeEsquerda);
        IR_PARA_FRENTE_ESQUERDA();
        delay(20);
        //while (analogRead(A0) < 200 || analogRead(A1) < 200);
        //while (analogRead(A0) < 200);
        //IR_PARA_FRENTE_DIREITA();
        FREIO();
        delay(20);
      }else if(analogRead(A4) > 200){  // Esquerda entrou na linha
        ACELERA_ESQUERDA(velocidadeEsquerda);
        IR_PARA_TRAS_ESQUERDA();
        ACELERA_DIREITA(velocidadeDireita);
        IR_PARA_FRENTE_DIREITA();
        delay(20);
        //frente = analogRead(A0);
        //tras = analogRead(A1);
        //while (analogRead(A0) < 200 || analogRead(A1) < 200);
        //while (analogRead(A0) < 200);
        //IR_PARA_FRENTE_ESQUERDA();
        FREIO();
        delay(20);
      }
  }if (analogRead(A0) < 200 && analogRead(A1) < 200 && analogRead(A2) < 200 && analogRead(A3), analogRead(A4)){ // Frente e trás sairam da linha
    //Serial.println("parar");
    FREIO();
    delay(500);
  }
  /* Considerando que o robo comece na posição 8 virado para cima */
/*  if (analogRead(A0) > 200 && analogRead(A1) > 200 && analogRead(A2) > 200 && analogRead(A3) > 200 && analogRead(A4) > 200) {   // Cruzamento
    FREIO();
    delay(1000);
    ACELERA_ESQUERDA(velocidadeEsquerda);
    ACELERA_DIREITA(velocidadeEsquerda);
    IR_PARA_TRAS_DIREITA();
    IR_PARA_FRENTE_ESQUERDA();
    delay(250);
    FREIO();
  }else if(analogRead(A0) > 200 && analogRead(A1) > 200 && analogRead(A2) > 200 && analogRead(A3) > 200 && analogRead(A4) < 200){ // esquerda, frente, centro e tras
    
  }else if(analogRead(A0) > 200 && analogRead(A1) < 200 && analogRead(A2) > 200 && analogRead(A3) > 200 && analogRead(A4) < 200){ // esquerda, centro e tras
    
  }else if(analogRead(A0) > 200 && analogRead(A1) < 200 && analogRead(A2) > 200 && analogRead(A3) > 200 && analogRead(A4) > 200){  // esquerda, centro, tras e direita
    
  }else if(analogRead(A0) > 200 && analogRead(A1) > 200 && analogRead(A2) > 200 && analogRead(A3) < 200 && analogRead(A4) < 200){ // esquerda, frente e centro
    
  }else if(analogRead(A0) < 200 && analogRead(A1) < 200 && analogRead(A2) > 200 && analogRead(A3) > 200 && analogRead(A4) > 200){ // Centro, tras e direita
    
  }
  */
  //Serial.println(contador_direita);
  //Serial.println(contador_esquerda);
  /*
  ACELERA_DIREITA(velocidadeDireita);
  ACELERA_ESQUERDA(velocidadeEsquerda);
  IR_PARA_FRENTE_DIREITA();
  IR_PARA_FRENTE_ESQUERDA();
  delay(1000);
  FREIO();
  delay(1500);
  */
}

void contadorDireita(){
  contador_direita++;
}

void contadorEsquerda(){
  contador_esquerda++;
}

