#include <Arduino.h>
#include <LibRobus.h>
#include "Adafruit_TCS34725.h"
#include <stdlib.h>
#include "SharpIR.h"
#include <Wire.h> 
#include <LiquidCrystal.h>


#define IRPin A3
#define model 1080

//delaration fonctions
void Service_Cafe();

//LiquidCrystal lcd(4, 7, 8, 9, 10, 11, 12);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // LCD Shield

const byte backLightpin = 5; 
//const byte backLightpin = 10; 
const byte contrast_pin = 6;

//delaration pins
const int pinBout1 = 22;
const int pinBout2 = 23;
const int pinBout3 = 24;


void setup ()
{
  
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
  Service_Cafe();
}


void Service_Cafe() {
  int distance_cm;
  int valeurB1, valeurB2, valeurB3;     //b1 = +  b2 = -  b3 = enter
  int comptLait = 0;
  int comptSucre = 0;

  valeurB1 = digitalRead(pinBout1);
  valeurB2 = digitalRead(pinBout2);
 
  /*Serial.print("Bouton 1 : ");
  Serial.print(valeurB1);
  Serial.print(" Bouton 2 : ");
  Serial.print(valeurB2);*/

  lcd.setCursor(0, 0);
  lcd.print("Voulez vous du cafe ?");
  lcd.setCursor(0, 1);
  lcd.print(" Oui        Non ");
  
  while(valeurB1 == false && valeurB2 == false)
  {
    valeurB1 = digitalRead(pinBout1);
    valeurB2 = digitalRead(pinBout2);
  }
  
  if(valeurB2 == true)
  {
    String message = "Bonne journee !";
    int position = (16 - message.length()) / 2;
    
    lcd.clear();
    lcd.setCursor(position-1, 0);    // pour centrer le texte  -1 car la premiere position est 0 et non 1
    lcd.print(message);
    delay(3500);
    return;     //sort de la fonction
  }
  else if(valeurB1 == true)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Posez le verre");
    lcd.setCursor(0, 1);
    lcd.print("sur le plateau");
  }
  
  while(distance_cm>10)
  {
    SharpIR mySensor = SharpIR(IRPin, model);
    distance_cm = mySensor.distance();
    delay(3);
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Du lait ?");
  lcd.setCursor(14, 1);
  lcd.print(comptLait);
  lcd.setCursor(15, 0);
  lcd.print("+"); 
  lcd.setCursor(15, 1);
  lcd.print("-");

  while(valeurB3 == false)    //tant que l'utilisateur ne pese pas sur enter
  {
    valeurB1 = digitalRead(pinBout1);
    valeurB2 = digitalRead(pinBout2);
    valeurB3 = digitalRead(pinBout3);
    delay(1);

    if(valeurB1 == true)
    {
      comptLait++;
      if(comptLait > 3)  //limite de 3 laits
        comptLait = 3;
      
      lcd.setCursor(14, 1);
      lcd.print(comptLait);
      while(digitalRead(pinBout1)==true);   //attendre que l'utilisateur relache le bouton
    }
    else if(valeurB2 == true)
    {
      comptLait--;
      if(comptLait < 0)  //limite de 0 laits
        comptLait = 0;

      lcd.setCursor(14, 1);
      lcd.print(comptLait);
      while(digitalRead(pinBout2)==true);   //attendre que l'utilisateur relache le bouton
    }
    
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voici le lait");
  for(int i = 0; i < comptLait; i++)
  {
    //////////////// fonction pour donner le lait ici////////////////////
    delay(500);
  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Du sucre ?");
  lcd.setCursor(14, 1);
  lcd.print(comptSucre);
  lcd.setCursor(15, 0);
  lcd.print("+"); 
  lcd.setCursor(15, 1);
  lcd.print("-");

  while(valeurB3 == false)    //tant que l'utilisateur ne pese pas sur enter
  { 
    valeurB1 = digitalRead(pinBout1);     
    valeurB2 = digitalRead(pinBout2);
    valeurB3 = digitalRead(pinBout3);
    delay(1);

    if(valeurB1 == true)
    {
      comptSucre++;
      if(comptSucre > 3)  //limite de 3 sucres
        comptSucre = 3;

      lcd.setCursor(14, 1);
      lcd.print(comptLait);
      while(digitalRead(pinBout1)==true);   //attendre que l'utilisateur relache le bouton
    }
    else if(valeurB2 == true)
    {
      comptLait--;
      if(comptSucre < 0)  //limite de 0 sucres
        comptSucre = 0;

      lcd.setCursor(14, 1);
      lcd.print(comptLait);
      while(digitalRead(pinBout2)==true);   //attendre que l'utilisateur relache le bouton
    }
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Voici le sucre");
  for(int i = 0; i < comptSucre; i++)
  {
    //////////////// fonction pour donner le sucre ici////////////////////
    delay(500);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Prenez le verre");
  lcd.setCursor(0, 1);
  lcd.print("Bonne journee !");
  
  while(distance_cm < 10)
  {
    SharpIR mySensor = SharpIR(IRPin, model);
    distance_cm = mySensor.distance();
    delay(1);
  }
  return;
}
  /*
  while(distance_cm > 10) {

    SharpIR mySensor = SharpIR(IRPin, model);
    distance_cm = mySensor.distance();
    delay(3);

    if (distance_cm < 10) {

      lcd.clear();
      lcd.print("Du cafe ?");
      lcd.setCursor(13, 1);
      lcd.print("Oui");
      lcd.setCursor(13, 2);
      lcd.print("Non");

      //Si usager ne veut pas de cafe
      if (valeurB2 == true) {

        lcd.clear();
        lcd.print("Bonne journee !");
        delay(2000);
        //code de retour sur la ligne noire centrale ici
      }

      //Si usager veut du cafe
      if (valeurB1 == true) {

        lcd.clear();
        //code de la pompe pour le cafe ici
        delay(2000);
        lcd.print("Du lait ?");
        lcd.setCursor(13, 1);
        lcd.print("Oui");
        lcd.setCursor(13, 2);
        lcd.print("Non");

        //Si usager veut du lait
        if (valeurB1 == true) {
          lcd.clear();
          lcd.setCursor(1, 1);
          lcd.print("1 lait ?");
          lcd.setCursor(1, 2);
          lcd.print("2 lait ?");

          //Si usager veut 1 lait
          if (valeurB1 == true) {
            lcd.clear();
            //code de la pompe pour le lait (delay pour 1 lait)
            delay(2000);
            lcd.print("Du sucre ?");

            //Si usager veut du sucre
            if (valeurB1 == true) {
              lcd.clear();
              lcd.setCursor(1, 1);
              lcd.print("1 sucre ?");
              lcd.setCursor(1, 2);
              lcd.print("2 sucre ?");

              //Si usager veut 1 sucre
              if (valeurB1 == true) {

                lcd.clear();
                //code servomoteurs pour le sucre (delay pour 1 sucre)
                delay(2000);

                //fin de la distribution
                while(distance_cm < 10) {
                  lcd.clear();
                  lcd.print("Prenez le verre");
                  SharpIR mySensor = SharpIR(IRPin, model);
                  distance_cm = mySensor.distance();
                  delay(1);

                  if (distance_cm > 10) {

                    lcd.clear();
                    lcd.print("Bonne journee !");
                    delay(2000);
                    //code de retour sur la ligne noire centrale ici
                  }
                }
              }

              //Si usager veut 2 sucre
              if (valeurB2 == true) {

                lcd.clear();
                //code servomoteurs pour le sucre (delay pour 2 sucre)
                delay(2000);

                //fin de la distribution
                while(distance_cm < 10) {
                  lcd.clear();
                  lcd.print("Prenez le verre");
                  SharpIR mySensor = SharpIR(IRPin, model);
                  distance_cm = mySensor.distance();
                  delay(1);

                  if (distance_cm > 10) {

                    lcd.clear();
                    lcd.print("Bonne journee !");
                    delay(2000);
                    //code de retour sur la ligne noire centrale ici
                  }
                }
              }
            }

            //Si usager veut pas de sucre
            if (valeurB2 == true) {
              while(distance_cm < 10) {
                lcd.clear();
                lcd.print("Prenez le verre");
                SharpIR mySensor = SharpIR(IRPin, model);
                distance_cm = mySensor.distance();
                delay(1);

                if (distance_cm > 10) {

                  lcd.clear();
                  lcd.print("Bonne journee !");
                  delay(2000);
                  //code de retour sur la ligne noire centrale ici
                }
              }
            }
          }

          //Si usager veut 2 lait
          if (valeurB2 == true) {
            lcd.clear();
            //code de la pompe pour le lait (delay pour 2 lait)
            delay(2000);
            lcd.print("Du sucre ?");

            //Si usager veut du sucre
            if (valeurB1 == true) {
              lcd.clear();
              lcd.setCursor(1, 1);
              lcd.print("1 sucre ?");
              lcd.setCursor(1, 2);
              lcd.print("2 sucre ?");

              //Si usager veut 1 sucre
              if (valeurB1 == true) {

                lcd.clear();
                //code servomoteurs pour le sucre (delay pour 1 sucre)
                delay(2000);

                //fin de la distribution
                while(distance_cm < 10) {
                  lcd.clear();
                  lcd.print("Prenez le verre");
                  SharpIR mySensor = SharpIR(IRPin, model);
                  distance_cm = mySensor.distance();
                  delay(1);

                  if (distance_cm > 10) {

                    lcd.clear();
                    lcd.print("Bonne journee !");
                    delay(2000);
                    //code de retour sur la ligne noire centrale ici
                  }
                }
              }

              //Si usager veut 2 sucre
              if (valeurB2 == true) {

                lcd.clear();
                //code servomoteurs pour le sucre (delay pour 2 sucre)
                delay(2000);

                //fin de la distribution
                while(distance_cm < 10) {
                  lcd.clear();
                  lcd.print("Prenez le verre");
                  SharpIR mySensor = SharpIR(IRPin, model);
                  distance_cm = mySensor.distance();
                  delay(1);

                  if (distance_cm > 10) {

                    lcd.clear();
                    lcd.print("Bonne journee !");
                    delay(2000);
                    //code de retour sur la ligne noire centrale ici
                  }
                }
              }
            }

            //Si usager veut pas de sucre
            if (valeurB2 == true) {
              while(distance_cm < 10) {
                lcd.clear();
                lcd.print("Prenez le verre");
                SharpIR mySensor = SharpIR(IRPin, model);
                distance_cm = mySensor.distance();
                delay(1);

                if (distance_cm > 10) {

                  lcd.clear();
                  lcd.print("Bonne journee !");
                  delay(2000);
                  //code de retour sur la ligne noire centrale ici
                }
              }
            }
          }
        }

        //--------------------------------------------------------------------------------------------------------------

        //Si usager veut pas de lait
        if (valeurB2 == true) {

          lcd.clear();
          lcd.print("Du sucre ?");
          lcd.setCursor(13, 1);
          lcd.print("Oui");
          lcd.setCursor(13, 2);
          lcd.print("Non");

          //Si usager veut du sucre
          if (valeurB1 == true) {
            lcd.clear();
            lcd.setCursor(1, 1);
            lcd.print("1 sucre ?");
            lcd.setCursor(1, 2);
            lcd.print("2 sucre ?");

            //Si usager veut 1 sucre
            if (valeurB1 == true) {

              lcd.clear();
              //code servomoteurs pour le sucre (delay pour 1 sucre)
              delay(2000);

              //fin de la distribution
              while(distance_cm < 10) {
                lcd.clear();
                lcd.print("Prenez le verre");
                SharpIR mySensor = SharpIR(IRPin, model);
                distance_cm = mySensor.distance();
                delay(1);

                if (distance_cm > 10) {

                  lcd.clear();
                  lcd.print("Bonne journee !");
                  delay(2000);
                  //code de retour sur la ligne noire centrale ici
                }
              }
            }

            //Si usager veut 2 sucre
            if (valeurB2 == true) {

              lcd.clear();
              //code servomoteurs pour le sucre (delay pour 2 sucre)
              delay(2000);

              //fin de la distribution
              while(distance_cm < 10) {
                lcd.clear();
                lcd.print("Prenez le verre");
                SharpIR mySensor = SharpIR(IRPin, model);
                distance_cm = mySensor.distance();
                delay(1);

                if (distance_cm > 10) {

                  lcd.clear();
                  lcd.print("Bonne journee !");
                  delay(2000);
                  //code de retour sur la ligne noire centrale ici
                }
              }
            }
          }

          //Si usager veut pas de sucre
          if (valeurB2 == true) {
            while(distance_cm < 10) {
              lcd.clear();
              lcd.print("Prenez le verre");
              SharpIR mySensor = SharpIR(IRPin, model);
              distance_cm = mySensor.distance();
              delay(1);

              if (distance_cm > 10) {

                lcd.clear();
                lcd.print("Bonne journee !");
                delay(2000);
                //code de retour sur la ligne noire centrale ici
              }
            }
          }
        }
      }
    }
  }*/




