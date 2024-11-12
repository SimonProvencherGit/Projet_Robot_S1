#include <Arduino.h>
#include <LibRobus.h>
#include "Adafruit_TCS34725.h"
#include <stdlib.h>
#include "SharpIR.h"
#include <Wire.h> 
#include <LiquidCrystal.h>

//delaration fontions
void UI();

//LiquidCrystal lcd(4, 7, 8, 9, 10, 11, 12);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // LCD Shield


const byte backLightpin = 5; 
//const byte backLightpin = 10; 
const byte contrast_pin = 6;

//delaration pins
const int pinBout1 = 22;
const int pinBout2 = 23;
const int pinBout3 = 24;

void setup () {
  
  pinMode(pinBout1, INPUT);
  pinMode(pinBout2, INPUT);
  pinMode(pinBout3, INPUT);

  //initilisation des pins
   //lcd.begin(16,2);   
   lcd.begin(16,2);
   lcd.clear();
  // lcd.backlight();
  //   lcd.setBacklightPin(3);
   //  lcd.setBacklight(1);
   pinMode(backLightpin, OUTPUT);
   analogWrite(backLightpin, 210);
   pinMode(contrast_pin, OUTPUT);
   analogWrite(contrast_pin, 40);
  }  

void loop () {


  UI();
  
}


void UI()
{
  int valeurB1;
  int valeurB2;
  int valeurB3;

  valeurB1 =  digitalRead(pinBout1);
  valeurB2 =  digitalRead(pinBout2);
  valeurB3 =  digitalRead(pinBout3);

 
  Serial.print("Bouton 1 : ");
  Serial.print(valeurB1);
  Serial.print(" Bouton 2 : ");
  Serial.print(valeurB2);
  Serial.print(" Bouton 3 : ");
  Serial.println(valeurB3);

  while(tasse pas sur plateau)
  {
    faire lecture capteur distance 
    delay(1);

    si <5 on sait que la tasse est presente on sort du while
  }
  afficher cd de lait

  while(enter pas pese)
  {
    if(bouton augmente pese)
    {
      nblait++;
    }
    if(bouton diminue pese)
    {
      nblait--;
    }
    if(enter)
    {
      sort boulce
    }
  }
  distribuer lait

  meme code pour sucre   

  a la final

  while tasse encore presente  {
    prendre valeur capteur distance
    si <5 on sait que la tasse est presente on sort du while
  }
  si tasse plus la sort du code UI
}