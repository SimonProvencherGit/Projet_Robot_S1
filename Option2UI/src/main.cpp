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
void tournerAngleGauche(int angle);
void suiveurLigne();
void prendreValeurSuiveur();
void trouverLigne();
void trouverLigneExtremite();
int detecteurApp1();
void getdistance(int pinCaptDist);

//LiquidCrystal lcd(4, 7, 8, 9, 10, 11, 12);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // LCD Shield

const byte backLightpin = 5; 
//const byte backLightpin = 10; 
const byte contrast_pin = 6;

//delaration pins
const int pinBout1 = 22;
const int pinBout2 = 23;
const int pinBout3 = 24;

int vitesse = 1;
int capt1,capt2,capt3;
bool debut = true;
int vertpin = 23;
int rougepin = 22;
int distance_plateau = 0;
int distance_tasse = 0;

void setup ()
{
  pinMode(pinBout1, INPUT);       //initialisation des boutons
  pinMode(pinBout2, INPUT);
  pinMode(pinBout3, INPUT);
  
  pinMode(A0,INPUT);              //initialisation des capteurs de détecteurs de ligne
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);

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
  bool ligneDetecte = false;
  bool sertCafe = false;

  if(debut == true)
  {
    trouverLigne();
    debut = false;
  }

  while(sertCafe != true)
  {
      suiveurLigne();
      //fonction pour suivre la ligne
      //sample les suiveurs de ligne  (ds foncion suiveur le ligne)
      //sample les capteurs de distances et capteurs de proximité (ds fonction suiveur de ligne)

      //si capteur de proximité detecte un objet  (ds fonction suiveur de ligne)
      //arreter, reculer un peu, tourner 90 avancer un peu s'il n'y a rien devant pour faire face a l'usager
    
      tournerAngleGauche(180); 
      trouverLigne();
  }
  
  Service_Cafe();
  //code seuqence pour reculer et se retourner vers la ligne
  trouverLigneExtremite();
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
  lcd.print("Du cafe ?");
  lcd.setCursor(13, 0);
  lcd.print("Oui"); 
  lcd.setCursor(13, 1);
  lcd.print("Non");
  
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
    lcd.print("Bonne journee !");
    delay(3500);
    return;     //sort de la fonction pour retourner au main pour reculer et retrouver la ligne noire
  }
  else if(valeurB1 == true)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Posez le verre");
    lcd.setCursor(0, 1);
    lcd.print("sur le plateau");
  }
  
  while(distance_plateau>10)
  {
    getdistance(IRPin);   //mettre la pin du capteur du plateau
    delay(3);
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voici le cafe");
  
  delay(3000); //attendre 3 seconde pour que le verre soit bien place et pas commencer a verser le cafe trop tot
  
  ///////////////////////////////////////////////////// fonction pour donner le cafe ici////////////////////////////////////////////////////////

  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Du lait ?");
  lcd.setCursor(14, 1);
  lcd.print(comptLait);
  lcd.setCursor(15, 0);
  lcd.print("+"); 
  lcd.setCursor(15, 1);
  lcd.print("-");

  while(valeurB3 != true)    //tant que l'utilisateur ne pese pas sur enter
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
    //////////////////////////////////////////// fonction pour donner le lait ici//////////////////////////////////////////////////////////////
    delay(300);
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
    ///////////////////////////////////////////////////// fonction pour donner le sucre ici///////////////////////////////////////////////////////
    delay(300);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Prenez le verre");
  lcd.setCursor(0, 1);
  lcd.print("Bonne journee !");
  
  while(distance_plateau < 10)
  {
    getdistance(IRPin);   //mettre la pin du capteur du plateau
    delay(3);
  }
  delay(1000);    //attendre 1 seconde pour que l'utilisateur aie le temps de pendre son verre avant que le robot quitte

  return;   //retourne au main pour reculer et retrouver la ligne noire
}

void suiveurLigne()
{
  bool sort = false;
  int i = 0;
  int detecteurDevant;
  while(sort == false)
  {
    prendreValeurSuiveur();
    
    detecteurDevant = detecteurApp1();
    getdistance(23);  //mettre le pin du capteur de distance sur le cote du robot


    if(capt1 == 0 && capt2 == 1 && capt3 == 0)
    {
        MOTOR_SetSpeed(RIGHT,0.3*vitesse);
        MOTOR_SetSpeed(LEFT,0.3*vitesse);
    }
    else if ((capt1 == 1 && capt2 == 1 && capt3 == 0) || (capt1 == 1 && capt2 == 0 && capt3 == 0))
    {
        MOTOR_SetSpeed(RIGHT,0.3*vitesse);
        MOTOR_SetSpeed(LEFT,0);
    }
    else if ((capt1 == 0 && capt2 == 1 && capt3 == 1) || (capt1 == 0 && capt2 == 0 && capt3 == 1))
    {
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0.3*vitesse);
    }
    else if(capt1 == 1 && capt2 == 1 && capt3 == 1)
    {
        sort = true;
        delay(500);
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);
        return;
        //delay(500);
        //while(1);
    }
    else if((capt1 == 0 && capt2 == 0 && capt3 == 0) )
    {
      i++;
      if(i>=100)    //si on voit pas de ligne pour un petit bout on sort de suiveur ligne
      {
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);
        sort = true;
        return;
      }
    }
    if(detecteurDevant == 1)      //si on voit qqlch a gauche 
    {
      MOTOR_SetSpeed(RIGHT,0);    //insert code pour tourner dans le sens qu'il voit qqlch
      MOTOR_SetSpeed(LEFT,0);
      sort = true;
      return;
    }
    else if(detecteurDevant == 2) //si on voit qqlch a droite
    {
      MOTOR_SetSpeed(RIGHT,0);
      MOTOR_SetSpeed(LEFT,0);
      sort = true;
      return;
    }
    else if(detecteurDevant == 3) //si on voit qqlch devant
    {
      MOTOR_SetSpeed(RIGHT,0);
      MOTOR_SetSpeed(LEFT,0);
      sort = true;
      return;
    } 
    else if(distance_tasse < 10)  //si le cateur de distance sur le cote voit qqlch
    {
      MOTOR_SetSpeed(RIGHT,0);
      MOTOR_SetSpeed(LEFT,0);
      sort = true;
      return;
    }
  }
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);
}

void prendreValeurSuiveur()
{
  capt1 = analogRead(A0);            //lecture des capteurs de détecteurs de ligne
  capt2 = analogRead(A1);
  capt3 = analogRead(A2);

  if(capt1<500)
    capt1 = 0;
  else if(capt1>=500)
    capt1 = 1;
  if(capt2<=30)
    capt2 = 0;
  else if(capt2>33)
    capt2 = 1;
  if(capt3<100)
    capt3 = 0;
  else if(capt3>=100)
    capt3 = 1;

  Serial.print(capt1,DEC);        //affiche les valeurs des capteurs de détecteurs de ligne
  Serial.print("   ");
  Serial.print(capt2,DEC);
  Serial.print("   ");
  Serial.print(capt3,DEC);
  Serial.println("   ");
}

void tournerAngleGauche(int angle)
{
  double seuilAngle;

  //calculs pour trouver la valeur des encodeurs pour l'angle voulu
  // 1905/90 = x/angle
  // x = 1905/90 * angle  = nb de pulses pour l'angle voulu


  seuilAngle = angle * (1905/90);

  int Md = 0;
  int Mg = 0;
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);

  while(Md<(seuilAngle*0.03))       //courbe d'acceleration commence par petite vitesse pour le premier 3% de la rotation
  {
    MOTOR_SetSpeed(RIGHT, 0.1);
    MOTOR_SetSpeed(LEFT, -0.1);
    Md = ENCODER_Read(RIGHT);
  }
    while(Md<(seuilAngle * 0.06))   //augmentation de la vitesse jusqu'a 6% de la rotation
  {
    MOTOR_SetSpeed(RIGHT, 0.2);
    MOTOR_SetSpeed(LEFT, -0.2);
    Md = ENCODER_Read(RIGHT);
  }
  while(Md<(seuilAngle*0.94))    //vitesse maximale jusqu'a 94% de la rotation
  {
    MOTOR_SetSpeed(RIGHT, 0.3);
    MOTOR_SetSpeed(LEFT, -0.3-0.00535);
    Md = ENCODER_Read(RIGHT);
  }
  while(Md<seuilAngle)        //ralentissement pour les derniers 6% de la rotation
  {
    MOTOR_SetSpeed(RIGHT, 0.1);
    MOTOR_SetSpeed(LEFT, -0.1);
    Md = ENCODER_Read(RIGHT);
    Mg = ENCODER_Read(LEFT);
  }
  //Mg = Mg -1;
  Mg = -1;
  while(Mg-Md >5)   //Tant que la difference entre les deux encodeurs est plus grande que 5 pulses les moteurs sont pas allignés
  {
    while(Mg>=Md)
    {
      MOTOR_SetSpeed(RIGHT, 0.0);
      MOTOR_SetSpeed(LEFT, 0.1);
      Mg = ENCODER_Read(LEFT)*-1;
      //Mg = ENCODER_Read(LEFT);
    }
    while(Mg<=Md)
    {
      MOTOR_SetSpeed(RIGHT, 0.0);
      MOTOR_SetSpeed(LEFT, -0.1);
      Mg = ENCODER_Read(LEFT)*-1;
      //Mg = ENCODER_Read(LEFT);
    }
  }
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);
  delay(175);
}

void trouverLigne()
{
  bool sortie = false;
  int lignedetecte = 0;

  while(sortie == false)
  {
    for(int i=0; i<55;i++)
    {
      MOTOR_SetSpeed(LEFT, 0.3);
      MOTOR_SetSpeed(RIGHT, 0.05);
      prendreValeurSuiveur();
      if(capt1 ==1 || capt3 == 1)
      {
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        sortie = true;
        //suiveurLigne();
        return;
      }
    }

    MOTOR_SetSpeed(LEFT,0.2);
    MOTOR_SetSpeed(RIGHT,0.2);
    delay(300);
    
    for(int i=0; i<85;i++)
    {
      MOTOR_SetSpeed(LEFT, 0.05);
      MOTOR_SetSpeed(RIGHT, 0.3);
      prendreValeurSuiveur();
      if(capt1 ==1 || capt3 == 1)
      {
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        sortie = true;
        //suiveurLigne();
        return;
      }
    }
    
    MOTOR_SetSpeed(LEFT,0.2);
    MOTOR_SetSpeed(RIGHT,0.2);
    delay(300);

  }
  MOTOR_SetSpeed(LEFT,0);
  MOTOR_SetSpeed(RIGHT,0);
}

void trouverLigneExtremite()      
{
  bool sortie = false;

  while(sortie == false)
  {
    prendreValeurSuiveur();

    if(capt1 == 0 && capt2 == 0 && capt3 == 0)
    {
      MOTOR_SetSpeed(LEFT,0.3);
      MOTOR_SetSpeed(RIGHT,0.3);

      prendreValeurSuiveur();
    }
    else if(capt1 == 1 && capt3==1)
    {
      tournerAngleGauche(60);
      //suiveurLigne();
      sortie = true;
      MOTOR_SetSpeed(LEFT,0);
      MOTOR_SetSpeed(RIGHT,0);
      return;
    }
    else if(capt1 == 1)
    {
      while(capt1 == 1)
      {
        MOTOR_SetSpeed(LEFT,0);
        MOTOR_SetSpeed(RIGHT,0.3);

        prendreValeurSuiveur();
      }
      while(capt1 != 1)
      {
        MOTOR_SetSpeed(LEFT,0);
        MOTOR_SetSpeed(RIGHT,0.3);

        prendreValeurSuiveur();
      }

      //suiveurLigne();
      sortie = true;
      MOTOR_SetSpeed(LEFT,0);
      MOTOR_SetSpeed(RIGHT,0);
      return;
    }
    else if(capt3 == 1)
    {
      //while(1);
      //suiveurLigne();
      sortie = true;
      MOTOR_SetSpeed(LEFT,0);
      MOTOR_SetSpeed(RIGHT,0);
      return;
    }
  }
}

int detecteurApp1(){   //return 0 si rien, 1 si a gauche, 2 si a droite, 3 si devant
  bool vert = false;
  bool rouge = false;

  vert = digitalRead(vertpin);
  rouge = digitalRead(rougepin);

  if(!vert && rouge)  //obstacle a gauche
    return 1;
  else if(vert && !rouge)  //obstacle a droite
    return 2;
  else if(!vert && !rouge)  //obstacle devant
    return 3;
  else
    return 0;
}

void getdistance(int pinCaptDist)
{
  int distanceTemp;
  SharpIR mySensor = SharpIR(pinCaptDist, model);
  distanceTemp = mySensor.distance();
  Serial.println(distanceTemp);
  
  if(pinCaptDist == A0)           //remplacer A0... par les pin des capteurs de distance
    distance_tasse = distanceTemp;
  else if(pinCaptDist == A1)
    distance_plateau = distanceTemp;
}
