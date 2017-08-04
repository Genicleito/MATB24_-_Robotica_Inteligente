#include <Servo.h>

Servo servoSuperior;
Servo servoInferior;
/*
void setup() {
  // put your setup code here, to run once:
  servoSuperior.attach(11);
  //servoInferior.attach(10);
}
*/
void levantarServoSuperior(){
  for(int i = 0; i < 60; i++){
    servoSuperior.write(i);
    delay(10);
  }
}

void abaixarServoSuperior(){
  for(int i = 60; i > 0; i--){
    servoSuperior.write(i);
    delay(10);
  }
}

void irParaDireitaServoInferior(){
  for(int i = 0; i < 170; i++){
    servoInferior.write(i);
    delay(10);
  }
}

void irParaEsquerdaServoInferior(){
  for(int i = 170; i > 0; i--){
    servoInferior.write(i);
    delay(10);
  }
}
/*
void loop() {
  //irParaDireitaServoInferior();
  //delay(500);
  levantarServoSuperior();
  delay(1000);
  //irParaEsquerdaServoInferior();
  //delay(500);
  abaixarServoSuperior();
  delay(1000);
}*/
