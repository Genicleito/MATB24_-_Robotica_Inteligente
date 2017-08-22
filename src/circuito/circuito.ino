#include <Ultrasonic.h>
#include <Servo.h> 

Servo servoInferior;
 
//Define os pinos para o trigger e echo
#define pino_trigger 3
#define pino_echo 2
 
//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);

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
//int velocidadeDireita  = 225; //200
int velocidadeDireita  = 160;
//int velocidadeEsquerda = 200; //180
int velocidadeEsquerda = 145;

void setup() {
  // Configuração da Comunicação Serial
  Serial.begin(9600);

  // Configuração dos pinos da Ponte H
  pinMode(DIRECAO_DIREITA_1, OUTPUT);
  pinMode(DIRECAO_DIREITA_2, OUTPUT);
  pinMode(DIRECAO_ESQUERDA_1, OUTPUT);
  pinMode(DIRECAO_ESQUERDA_2, OUTPUT);

  servoInferior.attach(10);

  // Configuração dos pinos do Encoder Ótico
//  pinMode(ENCODER_DIREITA, INPUT_PULLUP);
//  pinMode(ENCODER_ESQUERDA, INPUT_PULLUP);

  // Funções de Interrupção de cada um dos Encoders
//  attachInterrupt(digitalPinToInterrupt(ENCODER_DIREITA), contadorDireita, CHANGE);
//  attachInterrupt(digitalPinToInterrupt(ENCODER_ESQUERDA), contadorEsquerda, CHANGE);
  delay(2000);
}

bool girarServoEsquerda(){
  // delay(200);
  // long microsec = ultrasonic.timing();
  // distanciaFrente = ultrasonic.convert(microsec, Ultrasonic::CM);

  // servoInferior.write(180);
  // delay(200);

  // microsec = ultrasonic.timing();
  // distanciaEsquerda = ultrasonic.convert(microsec, Ultrasonic::CM);

  // servoInferior.write(90);
  // delay(200);
  long microsec;
  for(int i = 120; i <= 180; i += 5){
    servoInferior.write(i);
    delay(20);
    microsec = ultrasonic.timing();
    distanciaEsquerda = ultrasonic.convert(microsec, Ultrasonic::CM);
    if(distanciaEsquerda <= 10){
      //delay(100);
      servoInferior.write(90);
      delay(150);
      return true;
    }
    //delay(10);
  }
  //delay(100);
  servoInferior.write(90);
  delay(150);
  return false;
}

bool girarServoDireita(){
  long microsec;
  for(int i = 90; i > 0; i -= 5){
    delay(20);
    microsec = ultrasonic.timing();
    distanciaDireita = ultrasonic.convert(microsec, Ultrasonic::CM);
    if(distanciaDireita <= 10){
		FREIO();
      servoInferior.write(90);
      return true;
    }
    servoInferior.write(i);
    delay(10);
  }
  servoInferior.write(90);
  //delay(100);
  return false;
}

int x;
int distancia;
bool girarServo(){
  long microsec;
	int direction = 1;
	for(x = 90; ; x = (x+(5*direction))){

    	delay(20);
	    microsec = ultrasonic.timing();
    	distancia = ultrasonic.convert(microsec, Ultrasonic::CM);
	    if(distancia <= 10){
      		servoInferior.write(90);
			viraCalibrado(x);
    	  	return true;
	    }
	    servoInferior.write(x);
	    delay(10);
		if(x == 180){
			direction = -1;
		}else if(x == 90) {
			direction = 1;
		}
	}
	servoInferior.write(90);
	//delay(100);
	return false;
}

void loop() {
  ACELERA_DIREITA(velocidadeDireita);
  ACELERA_ESQUERDA(velocidadeEsquerda);
  IR_PARA_FRENTE_DIREITA();
  IR_PARA_FRENTE_ESQUERDA();

  	/*if(girarServoEsquerda()){
		viraDireita();
	}
	if(girarServoDireita()){
		viraEsquerda();
	}*/
  //girarServoDireita();

	girarServo();

  Serial.print("distanciaEsquerda: ");
  Serial.println(distanciaEsquerda);
  Serial.print("distanciaFrente: ");
  Serial.println(distanciaFrente);
  Serial.print("distanciaDireita: ");
  Serial.println(distanciaDireita);
  
  
}

void viraDireita(){

    Serial.println("==> Virando para direita...");
    ACELERA_DIREITA(velocidadeDireita + 40);
    ACELERA_ESQUERDA(velocidadeEsquerda + 30);
    IR_PARA_TRAS_DIREITA();
    IR_PARA_FRENTE_ESQUERDA();
    delay(250);
    FREIO();
    delay(100);

}

void viraEsquerda(){
    Serial.println("==> Virando para esquerda...");
    ACELERA_DIREITA(velocidadeDireita - 30);
    ACELERA_ESQUERDA(velocidadeEsquerda + 30);
    IR_PARA_FRENTE_DIREITA();
    IR_PARA_TRAS_ESQUERDA();
    delay(250);
    FREIO();
    delay(100);
}
void viraCalibrado(int x){
	
	int ajuste = abs(90 - x);
	if(x<=90){
	    Serial.println("==> Virando para esquerda...");
    	ACELERA_DIREITA(velocidadeDireita - 30);
    	ACELERA_ESQUERDA(velocidadeEsquerda + 30);
	    IR_PARA_FRENTE_DIREITA();
	    IR_PARA_TRAS_ESQUERDA();
	    delay(650 - (550/90)*ajuste );
	    FREIO();
	    delay(100);
	}

	if(x>90){
    	Serial.println("==> Virando para direita...");
	    ACELERA_DIREITA(velocidadeDireita);
	    ACELERA_ESQUERDA(velocidadeEsquerda);
	    IR_PARA_TRAS_DIREITA();
	    IR_PARA_FRENTE_ESQUERDA();
	    delay(650 - (550/90)*ajuste);
	    FREIO();
	    delay(100);
	}
}
void contadorDireita(){
  contador_direita++;
}

void contadorEsquerda(){
  contador_esquerda++;
}
