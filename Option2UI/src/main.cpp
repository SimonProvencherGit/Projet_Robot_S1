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
int getdistance(int pinCaptDist);
void Service_Cafe_TESTTESTTEST();
void lait();
void cafe();
void sucre();
void allerVersUsager();
void retourVersLigne();
void ligneTasse();
void suiveurLigneTest();
void testboutons();
void tournerAngleDroit(int angle);
void transitionSuiveurLigne();
void serviceCafeBackup();

//LiquidCrystal lcd(4, 7, 8, 9, 10, 11, 12);
//LiquidCrystal lcd(10,8,5,4,3,2);
LiquidCrystal lcd(44,42,40,38,39,41); // LCD Shield

//LiquidCrystal lcd(47,46,45,43,49,35);

//delaration pins
const int pinBout1 = A13;
const int pinBout2 = A11;
const int pinBout3 = A12;

int vitesse = 1;
int capt1,capt2,capt3;
bool debut = true;
int vertpin = 23;
int rougepin = 22;
bool sertCafe = false;
int pinDistCote = A1;
int pinDistPlateau = A0;

void setup ()
{
  BoardInit();
  Serial.begin(9600);

  //init temporaire
  pinMode(53, OUTPUT);

  pinMode(pinBout1, INPUT);       //initialisation des boutons
  pinMode(pinBout2, INPUT);
  pinMode(pinBout3, INPUT);
  
  pinMode(A2,INPUT);              //initialisation des capteurs de détecteurs de ligne
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);

  pinMode(pinDistCote, INPUT);     //initialisation des capteurs de distance
  pinMode(pinDistPlateau, INPUT);

  lcd.begin(16,2);
  lcd.clear();

  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);
  
  SERVO_Disable(0);
  SERVO_Disable(1);
  
  pinMode(A6, OUTPUT); // COFFEE
  digitalWrite(A6, HIGH); //ferme les moteurs pour le moment
  pinMode(A7, OUTPUT); // MILK
  digitalWrite(A7, HIGH);
}

void loop () 
{

  //int valeur,valeur2;
  //retourVersLigne();
  //Service_Cafe();
  //serviceCafeBackup();
  //while(1);
  /*while (1)
  {
    prendreValeurSuiveur();
    //tournerAngleDroit(90);
    delay(200);
  }*/
 //lcd.clear();
 //lcd.print("test test test");
 //while(1);

  /*
  cafe();
  delay(1500);
  lait();
  delay(1500);
  sucre();
  delay(1000);
  suiveurLigne();*/

  /*while(1)
  {
    //Serial.print("Cote: ");
    //valeur2 = getdistance(pinDistCote);
    //Serial.print(valeur2);
    //delay(100);
    
    Serial.print("  Plateau: ");
    valeur = getdistance(pinDistPlateau);
    Serial.println(valeur);
    
    if(valeur<=10)
      digitalWrite(53, HIGH);
    else
      digitalWrite(53, LOW);
  }*/

  lcd.clear();
  lcd.print("Recherche tasse");

  sertCafe = false;

  if(debut == true)
  {
    trouverLigne();
    debut = false;
  }

  while(sertCafe == false)
  {
    suiveurLigne();
    //suiveurLigneTest();

    if(sertCafe==false)
    {
      //tournerAngleDroit(210);
      tournerAngleDroit(185);
      trouverLigne();
    }
  }

  //while(1);
  
  allerVersUsager();

  Service_Cafe();
  //
  //serviceCafeBackup();
  //delay(2000);
  
  retourVersLigne();
}

void Service_Cafe() {
  bool valeurB1, valeurB2, valeurB3;     //b1 = +  b2 = -  b3 = enter
  int comptLait = 0;
  int comptSucre = 0;

  /*valeurB1 = digitalRead(pinBout1);
  valeurB2 = digitalRead(pinBout2);
 
  Serial.print("Bouton 1 : ");
  Serial.print(valeurB1);
  Serial.print(" Bouton 2 : ");
  Serial.print(valeurB2);*/

  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Du cafe ?    ");
  lcd.setCursor(13, 0);
  lcd.print("Oui"); 
  lcd.setCursor(13, 1);
  lcd.print("Non");

  valeurB1 = digitalRead(pinBout1);
  valeurB2 = digitalRead(pinBout2);
  valeurB3 = digitalRead(pinBout3);
  
  while(valeurB1 == 0 && valeurB2 == 0 && valeurB3 == 0)    //attent que l'usager pese sur un bouton pour choisir si il veut du cafe
  {
    valeurB1 = digitalRead(pinBout1);
    valeurB2 = digitalRead(pinBout2);
    valeurB3 = digitalRead(pinBout3);
    delay(1);
  }
  //while(digitalRead(pinBout1) == HIGH);
  //while(digitalRead(pinBout2) == HIGH);
  //while(digitalRead(pinBout3) == HIGH);

  if(valeurB2 == true)    //si l'usager ne veut pas de cafe
  {
    String message = "Bonne journee !";
    int position = (16 - message.length()) / 2;
    
    lcd.clear();
    lcd.setCursor(position-1, 0);    // pour centrer le texte  -1 car la premiere position est 0 et non 1
    lcd.print(message);
    delay(2500);
    return;     //sort de la fonction pour retourner au main pour reculer et retrouver la ligne noire
  }
  else if(valeurB1 == true || valeurB3 == true)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Posez le verre");
    lcd.setCursor(0, 1);
    lcd.print("sur le plateau");
  }
  
  /*while(getdistance(pinDistPlateau) <= 20)
  {
    //getdistance(pinDistPlateau);  
    delay(3);
  }*/
 int valeur;
 bool sortie = false;
 while(sortie == false)
  {
    valeur = getdistance(pinDistPlateau);
    
    if(valeur<=10)
    {
      digitalWrite(53, HIGH);
      sortie = true;
    }
    else
      digitalWrite(53, LOW);
  }
  
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voici le cafe");
  
  //delay(2000); 
  
  cafe();     // jsp s'il faudrait refaire une vérification pour le verre ici
  
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Du lait ?");
  lcd.setCursor(0, 1);
  lcd.print(comptLait);
  lcd.setCursor(15, 0);
  lcd.print("+"); 
  lcd.setCursor(15, 1);
  lcd.print("-");

  valeurB1 = digitalRead(pinBout1);
  valeurB2 = digitalRead(pinBout2);
  //valeurB3 = digitalRead(pinBout3);
  
  valeurB3 = false;

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
      
      lcd.setCursor(0, 1);
      lcd.print(comptLait);
      while(digitalRead(pinBout1)==HIGH);   //attendre que l'utilisateur relache le bouton
    }
    else if(valeurB2 == true)
    {
      comptLait--;
      if(comptLait < 0)  //limite de 0 laits
        comptLait = 0;

      lcd.setCursor(0, 1);
      lcd.print(comptLait);
      while(digitalRead(pinBout2)==HIGH);   //attendre que l'utilisateur relache le bouton
    }
    
  }
  while(digitalRead(pinBout3) == HIGH);

  if(comptLait != 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Voici le lait  ");
    
    for(int i = 0; i < comptLait; i++)
    {
      lait();
      delay(600);
    }
  }

  lcd.begin(16,2);  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Du sucre ?");
  lcd.setCursor(0, 1);
  lcd.print(comptSucre);
  lcd.setCursor(15, 0);
  lcd.print("+"); 
  lcd.setCursor(15, 1);
  lcd.print("-");

  //delay(100);
  //valeurB1 = digitalRead(pinBout1);
  //valeurB2 = digitalRead(pinBout2);
  //valeurB3 = digitalRead(pinBout3);
  valeurB3 = false;
  while(valeurB3 != true)    //tant que l'utilisateur ne pese pas sur enter
  { 
    valeurB1 = digitalRead(pinBout1);     
    valeurB2 = digitalRead(pinBout2);
    valeurB3 = digitalRead(pinBout3);
    //delay(10);

    if(valeurB1 == true)
    {
      comptSucre++;
      if(comptSucre > 3)  //limite de 3 sucres
        comptSucre = 3;

      lcd.setCursor(0, 1);
      lcd.print(comptSucre);
      while(digitalRead(pinBout1)==true);   //attendre que l'utilisateur relache le bouton
    }
    else if(valeurB2 == true)
    {
      comptSucre--;
      if(comptSucre < 0)  //limite de 0 sucres
        comptSucre = 0;

      lcd.setCursor(0, 1);
      lcd.print(comptSucre);
      while(digitalRead(pinBout2)==true);   //attendre que l'utilisateur relache le bouton
    }
  }
  while(digitalRead(pinBout3) == HIGH);
  
  if(comptSucre != 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Voici le sucre");

    for(int i = 0; i < comptSucre; i++)
    {
      sucre();
      delay(300);
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Prenez le verre");
  lcd.setCursor(0, 1);
  lcd.print("Bonne journee !");
  
  /*while(getdistance(pinDistPlateau) <= 20)
  {
    //getdistance(pinDistPlateau);  
    delay(3);
  }*/
  sortie = false;
  int valTemp;
  valTemp = getdistance(pinDistPlateau);

  while(sortie == false)
  {
    valeur = getdistance(pinDistPlateau);
    
    if(valeur>=25 && valeur != valTemp)
    {
      //digitalWrite(53, HIGH);
      sortie = true;
    }
  }

  delay(2000);    //attendre 1.5 seconde pour que l'utilisateur aie le temps de prendre son verre avant que le robot quitte
  
  lcd.clear();
  lcd.print("Recherche tasse");
  return;   //retourne au main pour reculer et retrouver la ligne noire
}

void suiveurLigne()
{
  bool sort = false;
  int i = 0;
  //int detecteurDevant;
  while(sort == false)
  {
    prendreValeurSuiveur();
    
    //detecteurDevant = detecteurApp1();
    //getdistance(IRPin);  //mettre le pin du capteur de distance sur le cote du robot

    if(capt1 == 0 && capt2 == 1 && capt3 == 0)
    {
        MOTOR_SetSpeed(RIGHT,0.3*vitesse);
        MOTOR_SetSpeed(LEFT,0.3*vitesse);
        i=0;
    }
    else if ((capt1 == 1 && capt2 == 1 && capt3 == 0) || (capt1 == 1 && capt2 == 0 && capt3 == 0))
    {
        MOTOR_SetSpeed(RIGHT,0.3*vitesse);
        MOTOR_SetSpeed(LEFT,0.1);
        i=0;
    }
    else if ((capt1 == 0 && capt2 == 1 && capt3 == 1) || (capt1 == 0 && capt2 == 0 && capt3 == 1))
    {
      MOTOR_SetSpeed(RIGHT,0.1);
      MOTOR_SetSpeed(LEFT,0.3*vitesse);
      i=0;
    }
    else if(capt1 == 0 && capt2 == 0 && capt3 == 0)
    {
      i++;
      if(i>=50)    //si on voit pas de ligne pour un petit bout on sort de suiveur ligne
      {
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);
        sort = true;
        return;
      }
    }

    if(getdistance(pinDistCote) < 20)  //si le cateur de distance sur le cote voit qqlch
    {
      MOTOR_SetSpeed(RIGHT,0);
      MOTOR_SetSpeed(LEFT,0);
      sort = true;
      sertCafe = true;
      return;
    }
  }
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);
}

void prendreValeurSuiveur()
{
  capt1 = analogRead(A4);            //lecture des capteurs de détecteurs de ligne
  capt2 = analogRead(A3);
  capt3 = analogRead(A2);

  if(capt1>=200)
    capt1 = 1;
  else if(capt1<200)
    capt1 = 0;

  if(capt2>=150)
    capt2 = 1;  
  else if(capt2<150)
    capt2 = 0;

  if(capt3>=40)
    capt3 = 1;
  else if(capt3<40)
    capt3 = 0;

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
  while(Mg-Md >10)   //Tant que la difference entre les deux encodeurs est plus grande que 5 pulses les moteurs sont pas allignés
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
    else if(capt1 == 1 && capt3 == 1)
    {
      MOTOR_SetSpeed(LEFT,0);
      MOTOR_SetSpeed(RIGHT,0);
      tournerAngleGauche(30);
      sortie = true;
      return;
    }
    else if(capt1 == 1 || capt3 == 1)
    {
      MOTOR_SetSpeed(LEFT,0);
      MOTOR_SetSpeed(RIGHT,0);
      sortie = true;
      return;
    }
    

    /*else if(capt1 == 1 && capt3==1)
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
    }*/
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

int getdistance(int pinCaptDist)
{
  int distanceTemp;
  SharpIR mySensor = SharpIR(pinCaptDist, model);
  distanceTemp = mySensor.distance();
  //Serial.println(distanceTemp);
  return distanceTemp;
}

void testboutons()
{
   bool valeurB1, valeurB2, valeurB3;
  while(1)
  {
    valeurB1 = digitalRead(pinBout1);
    valeurB2 = digitalRead(pinBout2);
    valeurB3 = digitalRead(pinBout3);
    Serial.print(valeurB1);
    Serial.print("  ");
    Serial.print(valeurB2);
    Serial.print("  ");
    Serial.println(valeurB3);
  }
}

void Service_Cafe_TESTTESTTEST() {
  
  bool valeurB1, valeurB2, valeurB3;     //b1 = +  b2 = -  b3 = enter
  int comptLait = 0;
  int comptSucre = 0;
  /*valeurB1 = digitalRead(pinBout1);
  valeurB2 = digitalRead(pinBout2);
 
  Serial.print("Bouton 1 : ");
  Serial.print(valeurB1);
  Serial.print(" Bouton 2 : ");
  Serial.print(valeurB2);*/
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Du cafe ?");
  lcd.setCursor(13, 0);
  lcd.print("Oui"); 
  lcd.setCursor(13, 1);
  lcd.print("Non");
  valeurB1 = digitalRead(pinBout1);
  valeurB2 = digitalRead(pinBout2);
  
  while(valeurB1 == 0 && valeurB2 == 0)    //attent que l'usager pese sur un bouton pour choisir si il veut du cafe
  {
    valeurB1 = digitalRead(pinBout1);
    valeurB2 = digitalRead(pinBout2);
    delay(1);
  }
  if(valeurB2 == true)    //si l'usager ne veut pas de cafe
  {
    String message = "Bonne journee !";
    int position = (16 - message.length()) / 2;
    
    lcd.clear();
    lcd.setCursor(position-1, 0);    // pour centrer le texte  -1 car la premiere position est 0 et non 1
    lcd.print(message);
    delay(3500);
    return;     //sort de la fonction pour retourner au main pour reculer et retrouver la ligne noire
  }
  else if(valeurB1 == true || valeurB3 == true)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Posez le verre");
    lcd.setCursor(0, 1);
    lcd.print("sur le plateau");
  }
  
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voici le cafe");
  
  delay(1000); //attendre 3 seconde pour que le verre soit bien place et pas commencer a verser le cafe trop tot
  
  ///////////////////////////////////////////////////// fonction pour donner le cafe ici////////////////////////////////////////////////////////
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Du lait ?");
  lcd.setCursor(0, 1);
  lcd.print(comptLait);
  lcd.setCursor(15, 0);
  lcd.print("+"); 
  lcd.setCursor(15, 1);
  lcd.print("-");
  valeurB1 = digitalRead(pinBout1);
  valeurB2 = digitalRead(pinBout2);
  valeurB3 = digitalRead(pinBout3);
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
      
      lcd.setCursor(0, 1);
      lcd.print(comptLait);
      while(digitalRead(pinBout1)==HIGH);   //attendre que l'utilisateur relache le bouton
    }
    else if(valeurB2 == true)
    {
      comptLait--;
      if(comptLait < 0)  //limite de 0 laits
        comptLait = 0;
      lcd.setCursor(0, 1);
      lcd.print(comptLait);
      while(digitalRead(pinBout2)==HIGH);   //attendre que l'utilisateur relache le bouton
    }
    
  }
  // Attendre que le bouton "enter" soit relâché
  while(digitalRead(pinBout3) == HIGH);
  if(comptLait != 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Voici le lait");
  }
  for(int i = 0; i < comptLait; i++)
  {
    //////////////////////////////////////////// fonction pour donner le lait ici//////////////////////////////////////////////////////////////
    delay(300);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Du sucre ?");
  lcd.setCursor(0, 1);
  lcd.print(comptSucre);
  lcd.setCursor(15, 0);
  lcd.print("+"); 
  lcd.setCursor(15, 1);
  lcd.print("-");
  //delay(100);
  //valeurB1 = digitalRead(pinBout1);
  //valeurB2 = digitalRead(pinBout2);
  valeurB3 = digitalRead(pinBout3);
  //valeurB3 = false;
  while(valeurB3 != true)    //tant que l'utilisateur ne pese pas sur enter
  { 
    valeurB1 = digitalRead(pinBout1);     
    valeurB2 = digitalRead(pinBout2);
    valeurB3 = digitalRead(pinBout3);
    //delay(10);
    if(valeurB1 == true)
    {
      comptSucre++;
      if(comptSucre > 3)  //limite de 3 sucres
        comptSucre = 3;
      lcd.setCursor(0, 1);
      lcd.print(comptSucre);
      while(digitalRead(pinBout1)==true);   //attendre que l'utilisateur relache le bouton
    }
    else if(valeurB2 == true)
    {
      comptSucre--;
      if(comptSucre < 0)  //limite de 0 sucres
        comptSucre = 0;
      lcd.setCursor(0, 1);
      lcd.print(comptSucre);
      while(digitalRead(pinBout2)==true);   //attendre que l'utilisateur relache le bouton
    }
  }
  // Attendre que le bouton "enter" soit relâché
  while(digitalRead(pinBout3) == HIGH);
  
  if(comptSucre != 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Voici le sucre");
  }
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
  
  delay(1000);    //attendre 1 seconde pour que l'utilisateur aie le temps de pendre son verre avant que le robot quitte
  return;   //retourne au main pour reculer et retrouver la ligne noire


}

void lait(){
  analogWrite(A7, LOW);
  delay(2000);
  digitalWrite(A7, HIGH);
}

void cafe(){
  digitalWrite(A6, LOW);
  delay(3500);
  digitalWrite(A6, HIGH);
}

void sucre ()
{
  SERVO_Enable(0);
  SERVO_SetAngle(0, 85);
  delay(1500);
  SERVO_Disable(0);   //a tester si on a besoin des enable et disable 
  SERVO_Enable(0);
  delay(1);
  SERVO_SetAngle(0, 110);
  delay(100);
  SERVO_Disable(0);
}

void allerVersUsager()
{
  MOTOR_SetSpeed(RIGHT,0.3);
  MOTOR_SetSpeed(LEFT,0.3);
  delay(500);
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);

  tournerAngleDroit(90);

  ligneTasse(); 
  return;
}

void retourVersLigne()
{
  bool sortie = false;
  MOTOR_SetSpeed(RIGHT, -0.3);
  MOTOR_SetSpeed(LEFT,-0.3);
  delay(300);
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);

  while(sortie == false)
  {
    prendreValeurSuiveur();

    if(capt1 == 0 && capt2 == 0 && capt3 == 0)
    {
      MOTOR_SetSpeed(LEFT,-0.3);
      MOTOR_SetSpeed(RIGHT,-0.3);

      prendreValeurSuiveur();
    }
    else if(capt1 == 1 && capt3 == 1)
    {
      MOTOR_SetSpeed(LEFT,0);
      MOTOR_SetSpeed(RIGHT,0);
      tournerAngleGauche(55);
      MOTOR_SetSpeed(LEFT,0.2);
      MOTOR_SetSpeed(RIGHT,0.2);
      delay(75);
      sortie = true;
      //return;
    }
  }
  sortie = false;

  /*while(sortie == false)
  {
    prendreValeurSuiveur();
    
    if(capt1 == 1 && capt3 ==1)
    {
      MOTOR_SetSpeed(RIGHT,-0.3);
      MOTOR_SetSpeed(LEFT,-0.3);
    }
    if(capt1 == 0 && capt3 == 0)
    {
      //delay(5);
      MOTOR_SetSpeed(RIGHT,0);
      MOTOR_SetSpeed(LEFT,0);
      //tournerAngleDroit(90);
      sortie = true;
    }
  }*/
  

  transitionSuiveurLigne();
  return;
}

void ligneTasse()
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
    else if(capt1 == 1 || capt3==1)
    { 
      MOTOR_SetSpeed(LEFT,0);
      MOTOR_SetSpeed(RIGHT,0);
      //while (1);
      sortie = true;
      return;
    }
    /*else if(capt1 == 1 && capt3==1)
    {
      sortie = true;
      MOTOR_SetSpeed(LEFT,0);
      MOTOR_SetSpeed(RIGHT,0);
      return;
    }
    else if(capt1 == 1)
    {
      while(capt3 != 1)
      {
        MOTOR_SetSpeed(LEFT,0);
        MOTOR_SetSpeed(RIGHT,0.3);

        prendreValeurSuiveur();
      }
      
      MOTOR_SetSpeed(RIGHT,0);
      
      return;
    }
    else if(capt3 == 1)
    {
     while(capt1 != 1)
      {
        MOTOR_SetSpeed(LEFT,0.3);
        MOTOR_SetSpeed(RIGHT,0);

        prendreValeurSuiveur();
      }

      MOTOR_SetSpeed(LEFT,0);
      
      return;
    }*/
  }
}

void suiveurLigneTest()
{
  bool sort = false;
  int i = 0;
  //int detecteurDevant;
  while(sort == false)
  {
    prendreValeurSuiveur();
    
    //detecteurDevant = detecteurApp1();
    //getdistance(IRPin);  //mettre le pin du capteur de distance sur le cote du robot

    if(capt1 == 0 && capt2 == 1 && capt3 == 0)
    {
        MOTOR_SetSpeed(RIGHT,0.3*vitesse);
        MOTOR_SetSpeed(LEFT,0.3*vitesse);
        i=0;
    }
    else if ((capt1 == 1 && capt2 == 1 && capt3 == 0) || (capt1 == 1 && capt2 == 0 && capt3 == 0))
    {
        MOTOR_SetSpeed(RIGHT,0.2*vitesse);
        MOTOR_SetSpeed(LEFT,0);
        i=0;
    }
    else if ((capt1 == 0 && capt2 == 1 && capt3 == 1) || (capt1 == 0 && capt2 == 0 && capt3 == 1))
    {
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0.2*vitesse);
        i=0;
    }
    /*else if(capt1 == 1 && capt2 == 1 && capt3 == 1)
    {
        sort = true;
        delay(500);
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);
        return;
        //delay(500);
        //while(1);
        i=0;
    }*/
    else if(capt1 == 0 && capt2 == 0 && capt3 == 0)
    {
      i++;
      delay(1);
      if(i>=350)    //si on voit pas de ligne pour un petit bout on sort de suiveur ligne
      {
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);
        sort = true;
        return;
      }
    }
    else if(capt1 == 1 && capt2 == 1 && capt3 ==1)
    {
      MOTOR_SetSpeed(RIGHT,0);
      MOTOR_SetSpeed(LEFT,0);
      
      tournerAngleGauche(30);
    }
  }
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);
}

void tournerAngleDroit(int angle) 
{
  double seuilAngle1;

  //calculs pour trouver la valeur des encodeurs pour l'angle voulu
  // 1905/90 = x/angle
  // x = 1905/90 * angle  = nb de pulses pour l'angle voulu

  seuilAngle1 = angle * (1905/90); 

  int Md = 0;
  int Mg = 0;
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);
  while(Mg<(seuilAngle1*0.03))
  {
    MOTOR_SetSpeed(LEFT, 0.1);
    MOTOR_SetSpeed(RIGHT, -0.1);
    Mg = ENCODER_Read(LEFT);
  }
    while(Mg<(seuilAngle1 * 0.06))
  {
    MOTOR_SetSpeed(LEFT, 0.2);
    MOTOR_SetSpeed(RIGHT, -0.2);
    Mg = ENCODER_Read(LEFT);
  }
  while(Mg<(seuilAngle1*0.94))
  {
    MOTOR_SetSpeed(LEFT, 0.3);
    MOTOR_SetSpeed(RIGHT, -0.3);
    Mg = ENCODER_Read(LEFT);
  }
  while(Mg<seuilAngle1)
  {
    MOTOR_SetSpeed(LEFT, 0.1);
    MOTOR_SetSpeed(RIGHT, -0.1);
    Mg = ENCODER_Read(LEFT);
    //Mg = ENCODER_Read(RIGHT);
  }
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
  Md = ENCODER_Read(RIGHT);
  Mg = ENCODER_Read(LEFT);
  //Mg = Mg -1;
  //Mg = -1;
  /*
  while(Mg-(Md*-1) > 50)   // car Md est l'inverse de l'encodeur gauche
  {
    if(Mg>(Md*-1))
    {
      MOTOR_SetSpeed(LEFT, 0);
      MOTOR_SetSpeed(RIGHT, -0.1);
      Md = ENCODER_Read(RIGHT);
    }
    else if(Mg<(Md*-1))
    {
      MOTOR_SetSpeed(LEFT, 0);
      MOTOR_SetSpeed(RIGHT, 0.1);
      Md = ENCODER_Read(RIGHT);
    }
  }*/
  /*while(Md-Mg >5)   //Tant que la difference entre les deux encodeurs est plus grande que 5 pulses
  {
    while(Mg>=Md)
    {
      MOTOR_SetSpeed(LEFT, 0.0);
      MOTOR_SetSpeed(RIGHT, 0.1);
      Mg = ENCODER_Read(RIGHT)*-1;
      //Mg = ENCODER_Read(LEFT);
    }
    while(Mg<=Md)
    {
      MOTOR_SetSpeed(LEFT, 0.0);
      MOTOR_SetSpeed(RIGHT, -0.1);
      Mg = ENCODER_Read(RIGHT)*-1;
      //Mg = ENCODER_Read(LEFT);
    }
  }*/
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);
  //delay(175);
}
  
void transitionSuiveurLigne()
{
  bool sort = false;
  int i = 0;
  //int detecteurDevant;
  while(sort == false)
  {
    prendreValeurSuiveur();

    if(capt1 == 0 && capt2 == 1 && capt3 == 0)
    {
      sort = true;
      return;
    }
    else if ((capt1 == 1 && capt2 == 1 && capt3 == 0) || (capt1 == 1 && capt2 == 0 && capt3 == 0))
    {
      MOTOR_SetSpeed(RIGHT,0.3*vitesse);
      MOTOR_SetSpeed(LEFT,0);
      i=0;
    }
    else if ((capt1 == 0 && capt2 == 1 && capt3 == 1) || (capt1 == 0 && capt2 == 0 && capt3 == 1))
    {
      MOTOR_SetSpeed(RIGHT,0);
      MOTOR_SetSpeed(LEFT,0.3*vitesse);
      i=0;
    }
  }
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);
}

void serviceCafeBackup()
{
  bool valeurB1, valeurB2, valeurB3;     //b1 = +  b2 = -  b3 = enter
  int comptLait = 0;
  int comptSucre = 0;


 int valeur;
 bool sortie = false;

 lcd.clear();
 lcd.print("service cafe");
 
 while(sortie == false)
  {
    valeur = getdistance(pinDistPlateau);
    
    if(valeur<=10)
    {
      digitalWrite(53, HIGH);
      sortie = true;
    }
    else
      digitalWrite(53, LOW);
  }
  
  delay(2000);
  
  //cafe();     
    
  for(int i = 0; i < 2; i++)
  {
    //lait();
    delay(600);
  }
  
  for(int i = 0; i < 2; i++)
  {
    sucre();
    delay(300);
  }
  


  sortie = false;
  int valTemp;
  valTemp = getdistance(pinDistPlateau);

  while(sortie == false)
  {
    valeur = getdistance(pinDistPlateau);
    
    if(valeur>=25 && valeur != valTemp)
    {
      //digitalWrite(53, HIGH);
      sortie = true;
    }
  }

  delay(2000);    //attendre 1.5 seconde pour que l'utilisateur aie le temps de prendre son verre avant que le robot quitte
  
  lcd.clear();
  lcd.print("Recherche tasse");
  
  
  return;   //retourne au main pour reculer et retrouver la ligne noire
}