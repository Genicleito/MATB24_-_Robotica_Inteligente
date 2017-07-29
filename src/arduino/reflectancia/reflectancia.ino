void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.print("FRENTE = ");
  Serial.println(analogRead(A0));
  
  Serial.print("TRAS = ");
  Serial.println(analogRead(A1));
  
  Serial.print("CENTRO = ");
  Serial.println(analogRead(A2));
  
  Serial.print("DIREITA = ");
  Serial.println(analogRead(A3));
  
  
  Serial.print("ESQUERDA = ");
  Serial.println(analogRead(A4));
  delay(1000);
  Serial.println();
}        
