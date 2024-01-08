#include <Arduino.h>

//Constence de Sensor
const int frontline = 9;
const int sensorLeft = 45;
const int ledleft = 42;
const int sensorRight = 48;
const int ledRight = 41;
const int backline = 10;

//constence Moteurs 
const int motorAP = 16; // Motor pin 1
const int motorAN = 15; // Motor pin 2
const int motorBP = 7;  // Motor pin 3
const int motorBN = 6;  // Motor pin 4

// constence PWM Moteurs
const int frequencyMotor = 3000; // LEDC frequency in Hz
const int ledChannel1 = 0;
const int ledChannel2 = 1;
const int ledChannel3 = 2;
const int ledChannel4 = 3;
const int resolution = 8; // LEDC resolution

// constence PWM LED infrarouge
const int ledchannel5 = 4; // LEDC channel
const int ledchannel6 = 5; // LEDC channel
const int frequencyLed = 38000; // LEDC frequency in Hz
const int IR_DETECT_THRESHOLD = 2;

//Variables changement d'État
bool lost = true; // début des états et si objet perdu
bool find = false;  //  si on trouve l'objet 
bool lineDetectFront = false; // si on détecte la ligne en avant
bool lineDetectBack = false;  // si on détecte la ligne en arriere

//Fonction du mécanisme du robot
void customSetup();
void avoidLineFront();
void avoidLineBack();
void backward();
void forward();
void forwardFind();
void stop();
void left();
void left();
void right();

// Fonction des états
void objectDetection();
void searchOpponant();
void pushOpponant();
void etatMaintenant();


void customSetup(){
  Serial.begin(115200); 
  // mettre moteur en mode OUTPUT + pin LED infrarouge
  pinMode(motorAP, OUTPUT);
  pinMode(motorAN, OUTPUT);
  pinMode(motorBP, OUTPUT);
  pinMode(motorBN, OUTPUT);
  pinMode(ledleft,OUTPUT);

  // pin de lecture
  pinMode(frontline, INPUT);
  pinMode(backline, INPUT);
  pinMode(sensorLeft,INPUT);
  pinMode(sensorRight,INPUT);
  
  // frequence des 4 pins de Moteurs + frequence LED infrarouge
  ledcSetup(ledChannel1, frequencyMotor, resolution);
  ledcSetup(ledChannel2, frequencyMotor, resolution);
  ledcSetup(ledChannel3, frequencyMotor, resolution);
  ledcSetup(ledChannel4, frequencyMotor, resolution);
  ledcSetup(ledchannel5, frequencyLed, resolution);
  ledcSetup(ledchannel6, frequencyLed, resolution);

  // Attach motor pins to the PWM channels + frequence LED infrarouge
  ledcAttachPin(motorAP, ledChannel1);
  ledcAttachPin(motorAN, ledChannel2);
  ledcAttachPin(motorBP, ledChannel3);
  ledcAttachPin(motorBN, ledChannel4);
  ledcAttachPin(ledleft, ledchannel5);
  ledcAttachPin(ledRight, ledchannel6);

  // affiche que tout est fini
  Serial.print("Setup Done\n");
}

void avoidLineFront(){  // fonction qui évite la ligne 
  stop();
  delay(500);
  backward();
  delay(500);
  stop();
  delay(500);
  left();
  delay(500);
  stop();
  delay(500);
}

void avoidLineBack(){
  stop();
  delay(500);
  forward();
  delay(500);
  stop();
  delay(500);
  right();
  delay(500);
  stop();
  delay(500);
}

void backward(){
  ledcWrite(ledChannel1, 200); // Motor A P
  ledcWrite(ledChannel2, 0); // Motor A N
  ledcWrite(ledChannel3, 200); // Motor B P
  ledcWrite(ledChannel4, 0); // Motor B N
}

void forward(){
  ledcWrite(ledChannel1, 0); // Motor A P
  ledcWrite(ledChannel2, 200); // Motor A N
  ledcWrite(ledChannel3, 0); // Motor B P
  ledcWrite(ledChannel4, 200); // Motor B N
}

void forwardFind(){
    ledcWrite(ledChannel1, 0); // Motor A P
    ledcWrite(ledChannel2, 190); // Motor A N
    ledcWrite(ledChannel3, 0); // Motor B P
    ledcWrite(ledChannel4, 190); // Motor B N
}

void stop(){
  ledcWrite(ledChannel1, 0); // Motor A P
  ledcWrite(ledChannel2, 0); // Motor A N
  ledcWrite(ledChannel3, 0); // Motor B P
  ledcWrite(ledChannel4, 0); // Motor B N
}

void left(){
  ledcWrite(ledChannel1, 180); // Motor A P
  ledcWrite(ledChannel2, 0); // Motor A N
  ledcWrite(ledChannel3, 0); // Motor B P
  ledcWrite(ledChannel4, 180); // Motor B N
}

void right(){
  ledcWrite(ledChannel1, 0); // Motor A P
  ledcWrite(ledChannel2, 180); // Motor A N
  ledcWrite(ledChannel3, 180); // Motor B P
  ledcWrite(ledChannel4, 0); // Motor B N
}

void objectDetection(){
   // Envoie du Signal
  ledcWrite(ledchannel5, 128); // PWM à 50%
  delay(60);                  
  ledcWrite(ledchannel5, 0); 
  delay(90);

  // Read IR receiver for object detection
  int val = digitalRead(sensorLeft);
  Serial.print(val);
  if (digitalRead(sensorLeft) == LOW) {  
    if (digitalRead(frontline)==LOW){
      find = true;
    }
    else{
      lineDetectFront = true;
    }
  }
  else if (digitalRead(sensorRight) == LOW) { 
    if (digitalRead(frontline)==LOW){
      find = true;
    }
    else{
      lineDetectFront = true;
    }
  }
  else{
    lost = true;
    find = false;
  }
}

void searchOpponant(){
  objectDetection();
}

void pushOpponant(){
  if(find){
    if(!lineDetectFront){
      forwardFind();
      lost=false;
    }
  }
  else if(lost){
    left();
    find= false;
  }

}

void etatMaintenant(){
  searchOpponant();
  // État d'éviter les lignes
  if(lineDetectFront){  // évite la ligne en avant
    Serial.print("Avoid_front\n");
    avoidLineFront();
    lineDetectFront = false;
  }
  // Recherche de l'adversaire
  else if(lost){
    Serial.print("Search\n");
    searchOpponant();
  }
  // État de pousser l'adversaire
  else if(find){
    Serial.print("push\n");
    pushOpponant();
  }
  delay(5);
}