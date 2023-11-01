#include <Arduino.h>

int positifA =4;
int negatifA =1;
int positifB = 5;
int negatifB = 2;

void setup(){
    Serial.begin(9600);
    Serial.print("test");
    pinMode(positifA, OUTPUT);
    pinMode(negatifA, OUTPUT);
    pinMode(positifB, OUTPUT);
    pinMode(negatifB, OUTPUT);
}

void forward(){
    digitalWrite(positifA,HIGH);
    digitalWrite(negatifA, LOW);
    digitalWrite(positifB, LOW);
    digitalWrite(negatifB, HIGH);
}

void backward(){
    digitalWrite(positifA, LOW);
    digitalWrite(negatifA,HIGH);
    digitalWrite(positifB, HIGH);
    digitalWrite(negatifB, LOW);
}

void loop(){
    forward();
    delay(1000);
    backward();
    delay(1000);
}
