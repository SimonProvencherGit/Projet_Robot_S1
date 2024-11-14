#include <Arduino.h>
#include <LibRobus.h>
#include "Adafruit_TCS34725.h"
#include <stdlib.h>
#include "SharpIR.h"
#include <Wire.h> 
#include <LiquidCrystal.h>

//delaration fonctions
void Service_Cafe();


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
   lcd.begin(16,2);
   lcd.clear();
}

void loop () {
  Service_Cafe();
}


void Service_Cafe() {
  int distance_cm;

  int valeurB1, valeurB2;

  valeurB1 = digitalRead(pinBout1);
  valeurB2 = digitalRead(pinBout2);

 
  Serial.print("Bouton 1 : ");
  Serial.print(valeurB1);
  Serial.print(" Bouton 2 : ");
  Serial.print(valeurB2);

  while(distance_cm > 10) {

    lcd.print("Posez le verre");
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
  }
}



