//MOTOR 1 BASE
#define M1I 22
#define M1D 24
#define EncoderM1 2
#define PWM1 6
#define iniM1 4
#define finM1 5
//MOTOR 2 ELEVADOR
#define M2I 26
#define M2D 28
#define EncoderM2 18
#define PWM2 9
#define iniM2 12
#define finM2 11
//MOTOR 3 CODO
#define M3I 40
#define M3D 42
#define PWM3 10
#define EncoderM3 19
#define finM3 44
//MOTOR 4 MUÑECA
#define M4I 36
#define M4D 38
#define EncoderM4 21//A1
#define PWM4 8
//TERMINAL
#define M5I 46
#define M5D 48
#define EncoderM5 3
#define PWM5 7

//Variables--------------------------------
int contador = 0;
void giraMotor(char, int, int, int, int, int, int, int);//funcion para girar motores grandes (direccion,velocidad,pulsos,pin 1 motor,pin 2 motor,switch inicio,switch final,pwm)
void giraMotorP(char, int, int, int, int, int, int, int, char);//funcion para girar motores pequeños (direccion,velocidad,pulsos,pin 1 motor,pin 2 motor,encoder mecanico,switch final,pwm,id motor)
void stopMotor(int, int, int);//funcion para detener motores (pin1 motor, pin2 motor, pwm)
void contar();//funcion contador de motores rojos
void contar2();//funcion contador motor codo
void contar3();//funcion contador motor muñeca
void contar4();//funcion contador motor terminal
unsigned long tiempoInicio = 0;//variable para terminar funcion de terminal


byte press_encoder1 = 0;
//------------------------------------------inicio arduino
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //motor base
  pinMode(M1I, OUTPUT);
  pinMode(M1D, OUTPUT);
  pinMode(finM1, INPUT);
  pinMode(iniM1, INPUT);
  pinMode(EncoderM1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EncoderM1), contar, CHANGE);
  //motor elevador
  pinMode(M2I, OUTPUT);
  pinMode(M2D, OUTPUT);
  pinMode(finM2, INPUT);
  pinMode(iniM2, INPUT);
  pinMode(EncoderM2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EncoderM2), contar, CHANGE);
  //motor codo
  pinMode(M3I, OUTPUT);
  pinMode(M3D, OUTPUT);
  pinMode(finM3, INPUT);
  pinMode(EncoderM3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EncoderM3), contar2, RISING);
  //motor muñeca
  pinMode(M4I, OUTPUT);
  pinMode(M4D, OUTPUT);
  pinMode(EncoderM4, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EncoderM4), contar3, CHANGE);
  //motor terminal
  pinMode(M5I, OUTPUT);
  pinMode(M5D, OUTPUT);
  pinMode(EncoderM5, INPUT_PULLUP); //
  attachInterrupt(digitalPinToInterrupt(EncoderM5), contar4, CHANGE);
  //inicializacion
  inicializa();
}

void loop() {
}
//-------------------------------------------------------------------------------------------------------------inicializar robot
void inicializa() {
  giraMotor('i', 155, 20000, M1D, M1I, iniM1, finM1, PWM1);//aprox 974
  giraMotor('s', 255, 164, M2D, M2I, iniM2, finM2, PWM2); //aprox 164          s=subir b=bajar
  giraMotorP('d', 230, 27, M3D, M3I, EncoderM3, finM3, PWM3, 'c');//27
  //giraMotorP('i', 60, 25, M4D, M4I, EncoderM4, 0, PWM4, 'm');//25
  giraMotorP('a', 255, 25, M5D, M5I, EncoderM5, 0, PWM5, 't');//20      a=abierto c=cerrado 3.5seg

}

//---------------------------------------------------------------------------------------------------------------motor negro pequeño
void giraMotorP(char dir, int vel, int pulsos, int motorD, int motorI, int em, int sw, int PWMg, char m) {//funcion para girar motores pequeños
  stopMotor(motorD, motorI, PWMg);//detiene motores
  if (dir == 'D' || dir == 'd' || dir == 'A' || dir == 'a') {//detecta direccion d=derecha a=abrir
    /*if (m == 'c') {
      Serial.println("iniciando giro derecha motor codo");
    } else if (m == 'm') {
      Serial.println("iniciando giro derecha motor muñeca");
    } else {
      Serial.println("iniciando giro derecha motor terminal");
    }//*/

    if (m == 'c' || m == 'C') {//si es motor del codo
      analogWrite(PWMg, vel);
      while (contador <= pulsos && digitalRead(sw) == false) {
        digitalWrite(motorI, HIGH);
        digitalWrite(motorD, LOW);
      }
    } else if (m == 'm' || m == 'M') {//si es motor de la muñeca
      analogWrite(PWMg, vel);
      while (contador <= pulsos ) {
        digitalWrite(motorI, HIGH);
        digitalWrite(motorD, LOW);
      }
    } else if (m == 't' || m == 'T') {//si es motor del terminal
      analogWrite(PWMg, vel);
      tiempoInicio = millis();
      while (contador <= pulsos ) {
        digitalWrite(motorI, HIGH);
        digitalWrite(motorD, LOW);
        if (millis() - tiempoInicio >= 3500) {//si pasan 3.5 segundos detiene el motor del terminal
          stopMotor(motorD, motorI, PWMg);
          contador = pulsos + 1;
          tiempoInicio = millis();
        }
      }
    }
  } else if (dir == 'I' || dir == 'i' || dir == 'C' || dir == 'c') {//detecta direccion i=izquierdo c=cerrar
    /*if (m == 'c') {
      Serial.println("iniciando giro izquierda motor codo");
    } else if (m == 'm') {
      Serial.println("iniciando giro izquierda motor muñeca");
    } else {
      Serial.println("iniciando giro izquierda motor terminal");
    }//*/
    if (m == 'c' || m == 'C') {//si es motor del codo
      analogWrite(PWMg, vel);
      while (contador <= pulsos) {

        digitalWrite(motorI, LOW);
        digitalWrite(motorD, HIGH);
      }
    } else if (m == 'm' || m == 'M') {//si es motor muñeca
      analogWrite(PWMg, vel);
      while (contador <= pulsos ) {
        digitalWrite(motorI, LOW);
        digitalWrite(motorD, HIGH);
      }
    } else if (m == 't' || m == 'T') {//si es motor del terminal
      analogWrite(PWMg, vel);
      tiempoInicio = millis();
      while (contador <= pulsos ) {
        digitalWrite(motorI, LOW);
        digitalWrite(motorD, HIGH);
        if (millis() - tiempoInicio >= 3500) {//si pasan 3.5 segundos detiene el motor del terminal
          stopMotor(motorD, motorI, PWMg);
          contador = pulsos + 1;
          tiempoInicio = millis();
        }
      }
    }
  }
  //Serial.println("Deteniendo motor: ");
  stopMotor(motorD, motorI, PWMg);//detener motores
}

//-------------------------------------------------------------------------------------------------------------motor rojo grande
void giraMotor(char dir, int vel, int pulsos, int motorD, int motorI, int inicio, int fin, int PWMg) {//funcion para girar motores grandes
  stopMotor(motorD, motorI, PWMg);//detiene motores
  analogWrite(PWMg, vel);
  if (dir == 'D' || dir == 'd' || dir == 's' || dir == 'S') {//detecta direccion d=derecha s=subir
    //Serial.println("iniciando giro DERECHA/SUBIR");
    while (contador <= pulsos && digitalRead(inicio) == false) {
      digitalWrite(motorI, LOW);
      digitalWrite(motorD, HIGH);
    }
  } else if (dir == 'I' || dir == 'i' || dir == 'b' || dir == 'B') {//detecta direccion i=izquierda b=bajar
    //Serial.println("iniciando giro IZQUIERDA/BAJAR");
    while (contador <= pulsos && digitalRead(fin) == false) {
      digitalWrite(motorI, HIGH);
      digitalWrite(motorD, LOW);
    }
  }
  //Serial.println("Deteniendo motor: ");
  stopMotor(motorD, motorI, PWMg);//detiene motores
}

//-------------------------------------------------------------------------------------------------------------detener motores
void stopMotor(int motorD, int motorI, int pwmg) {//con interrupciones cuenta pulsos
  contador = 0;
  analogWrite(pwmg, 0);
  digitalWrite(motorI, LOW);
  digitalWrite(motorD, LOW);
}

//-------------------------------------------------------------------------------------------------------------contador de pulsos
void contar() {//BASE Y ELEVADOR
  contador++;
  //Serial.print("contador: ");
  //Serial.println(contador);
}
void contar2() { //CODO cuenta pulsos mecanicos
  if (digitalRead(EncoderM3) == LOW) press_encoder1 = 1;
  if (digitalRead(EncoderM3) == HIGH && press_encoder1 == 1) {
    press_encoder1 = 0;
    contador++;
    //Serial.print("contador: ");
    //Serial.println(contador);
  }
}
void contar3() { //MUÑECA cuenta pulsos mecanicos
  if (digitalRead(EncoderM4) == LOW) press_encoder1 = 1;
  if (digitalRead(EncoderM4) == HIGH && press_encoder1 == 1) {
    press_encoder1 = 0;
    contador++;
    //Serial.print("contador: ");
    //Serial.println(contador);
  }
}
void contar4() { //TERMINAL cuenta pulsos mecanicos
  if (digitalRead(EncoderM5) == LOW) press_encoder1 = 1;
  if (digitalRead(EncoderM5) == HIGH && press_encoder1 == 1) {
    press_encoder1 = 0;
    contador++;
    //Serial.print("contador: ");
    //Serial.println(contador);
  }
}
