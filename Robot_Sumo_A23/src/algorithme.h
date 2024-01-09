#include <Arduino.h>

//Constence de Sensor
const int frontline = 9;
const int sensorLeft = 45;
const int ledleft = 42;
const int sensorRight = 48;
const int ledRight = 41;
const int backline = 10;

//constence Moteurs 
const int motorAP = 16; // Motor A pin 1
const int motorAN = 15; // Motor A pin 2
const int motorBP = 7;  // Motor B pin 1
const int motorBN = 6;  // Motor B pin 2

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


//Variables changement d'État
bool lost = true; // début des états et si objet perdu
bool find = false;  //  si on trouve l'objet 
bool lineDetectFront = false; // si on détecte la ligne en avant
bool lineDetectBack = false;  // si on détecte la ligne en arriere

//Fonction du mécanisme du robot
void customSetup();
void avoidLine();
void backward();
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


/*
* Fonction Setup pour le ESP32
*/
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


/*
* Fonction qui évite la ligne 
*/
void avoidLine(){  
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
  lineDetectFront = false;
}


/*
* Fonction pour reculer le robot
*/
void backward(){
  ledcWrite(ledChannel1, 200); // Motor A P
  ledcWrite(ledChannel2, 0); // Motor A N
  ledcWrite(ledChannel3, 200); // Motor B P
  ledcWrite(ledChannel4, 0); // Motor B N
}


/*
* Fonction pour avancer si adversaire détecté
*/
void forwardFind(){
    ledcWrite(ledChannel1, 0); // Motor A P
    ledcWrite(ledChannel2, 190); // Motor A N
    ledcWrite(ledChannel3, 0); // Motor B P
    ledcWrite(ledChannel4, 190); // Motor B N
}


/*
* Fonction pour arrêter le robot 
*/
void stop(){
  ledcWrite(ledChannel1, 0); // Motor A P
  ledcWrite(ledChannel2, 0); // Motor A N
  ledcWrite(ledChannel3, 0); // Motor B P
  ledcWrite(ledChannel4, 0); // Motor B N
}


/*
* Fonction pour tourner à gauche
*/
void left(){
  ledcWrite(ledChannel1, 180); // Motor A P
  ledcWrite(ledChannel2, 0); // Motor A N
  ledcWrite(ledChannel3, 0); // Motor B P
  ledcWrite(ledChannel4, 180); // Motor B N
}


/*
* Fonction pour tourner à droite
*/
void right(){
  ledcWrite(ledChannel1, 0); // Motor A P
  ledcWrite(ledChannel2, 180); // Motor A N
  ledcWrite(ledChannel3, 180); // Motor B P
  ledcWrite(ledChannel4, 0); // Motor B N
}


/*
* Fonction pour la détection d'objet
*/
void objectDetection(){
   // Envoie du Signal (Burst)
  ledcWrite(ledchannel5, 128); // PWM à 50%
  delay(60);                  
  ledcWrite(ledchannel5, 0);  //PWM à 0%
  delay(90);


  if (digitalRead(sensorLeft) == LOW) {   // si le capteur vois quelque chose a gauche
    if (digitalRead(frontline)==LOW){ // s'il ne détecte pas de ligne
      find = true;  // condition trouvé
      lost = false; // condition trouvé
    }
    else{
      lineDetectFront = true; // s'il voit une ligne
    }
  }
  else if (digitalRead(sensorRight) == LOW) {  // si le capteur vois quelque chose a droite
    if (digitalRead(frontline)==LOW){ // s'il ne détecte pas de ligne
      find = true;  // il le trouve
      lost = false; // pas perdu
    }
    else{
      lineDetectFront = true; // s'il voit une ligne
    } 
  }
  else{
    lost = true;  // condition perdu
    find = false; // condition perdu
  }
}


/*
* Fonction d'état de chercher l'adversaire
*/
void searchOpponant(){
  objectDetection();  // chercher l'adversaire
}


/*
* Fonction d'état de pousser l'adversaire
*/
void pushOpponant(){
  if(find){ // condition si trouvé 
    if(!lineDetectFront){ // si ne voit pas de ligne
      forwardFind();  // fonce dessus
    }
  }
  else if(lost){  // si perdu 
    find= false;  // condition perdu
  }
}


/*
* Fonction de modification d'état
*/
void etatMaintenant(){
  // État d'éviter les lignes
  if(!lineDetectFront){  // ne détect pas de ligne en avant

    // État de pousser l'adversaire
    if(find) {
      Serial.print("push\n");
      pushOpponant();
      find = false;
      lost = true;
    }

    // État de recherche de l'adversaire
    if(lost){
      Serial.print("Search\n");
      right();
      searchOpponant();
    }
  }

  // État d'éviter la ligne
  else {
    Serial.print("Avoid_front\n");
    avoidLine();
  }
}