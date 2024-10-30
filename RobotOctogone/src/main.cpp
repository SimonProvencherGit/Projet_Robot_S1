#include <Arduino.h>
#include <LibRobus.h>
#include "Adafruit_TCS34725.h"
#include <stdlib.h>
#include "SharpIR.h"

#define IRPin A3
#define model 1080

//declaration des fonctions
void tournerAngleDroit(int angle);
void tournerAngleGauche(int angle);
bool detectSiflet();
void premierVirage();
void allerCentreDebut();
void touverLigneExtremite();
void prendreValeurSuiveur();
void detectCouleur();
void suiveurLigne();
void TestTestTest();
void V2();
void tournantSuite(int nbOjetsButs);
void allerCentreSuite(int nbObjetsButs);
void scan90Gauche();
void scan90Droit();
void PrendreObjet();

// Déclaration des variables globales

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

int sifletAmbient = 35;
int siflet5Khz = 43;
bool depart = false;
bool bumperArriere;
uint8_t id = 3;
int vitesse = 1;
int nbObjetsButs = 0;
int capt1,capt2,capt3;
int couleur = 0;





void setup() {
  BoardInit();
  Serial.begin(9600);

  pinMode(A0,INPUT);              //initialisation des capteurs de détecteurs de ligne
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);

  SERVO_Disable(0);
  SERVO_Disable(1);

  if (tcs.begin()) {
        Serial.println("Found sensor");
    } else {
        Serial.println("No TCS34725 found ... check your connections");
       // while (1); // halt!
    }


}

 int distance_cm;

void ferme_bras()
{
  SERVO_Enable(1);
  SERVO_Enable(0);
  delay(5);
  SERVO_SetAngle(0,115);
  SERVO_SetAngle(1,65);
}

void getdistance()
  {
    SharpIR mySensor = SharpIR(IRPin, model);
    distance_cm = mySensor.distance();
    Serial.println(distance_cm);
  }

void loop() {

//scan90Gauche();
  PrendreObjet();
  while(1);
//scan90Gauche();
  // Detection d'objet


  //tournerAngleDroit(45);tournerAngleGauche((360-45));
  //premierVirage(4);
  //touverLigneExtremite();

  //scan90Gauche();
  //scan90Droit();
  //while(1);
  /*while(1)
  {
    detectCouleur();
    Serial.println(couleur);
    //prendreValeurSuiveur();
    delay(300);
  }*/
  //suiveurLigne();

  //V2();
  //touverLigneExtremite();


  bumperArriere = ROBUS_IsBumper(id);

  if(bumperArriere)
    depart = true;


  if(depart) {

    while(couleur == 0)
    {
      detectCouleur();
      //Serial.println(couleur);
      delay(5);
    }

    allerCentreDebut();

    if(nbObjetsButs == 0)   //tourner pour faire face au bon triangle
    {
      MOTOR_SetSpeed(LEFT,0);
      MOTOR_SetSpeed(RIGHT,0);

      //while(1);
      premierVirage();
      //premierVirage(2);
      //while(1);
    }
    else
    {
      tournantSuite(nbObjetsButs);
    }
    /*MOTOR_SetSpeed(LEFT,0.35);
    MOTOR_SetSpeed(RIGHT,0.3);
    delay(1000);
    MOTOR_SetSpeed(LEFT,0);
    MOTOR_SetSpeed(RIGHT,0);
    //while(1);
    touverLigneExtremite();*/

    MOTOR_SetSpeed(LEFT,0);
    MOTOR_SetSpeed(RIGHT,0);
    //while(1);
    //code pour trouver objet

    //code pour trouver la ligne sur l'extrimite et se diriger vers le but

    //code pour deposer l'objet dans le but et se replacer pour etre pret a repartir au centre
    // code doit, des qu'il voit de la couleur au lieu du blanc, reculer en pivorant sur la roue de droite pour
    //qu'il puisse avancer jusqu'a ce qu'il voit une ligne pour se rendre au centre

  }
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

void tournerAngleDroit(int angle)
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

  while(Mg<(seuilAngle*0.03))       //courbe d'acceleration commence par petite vitesse pour le premier 3% de la rotation
  {
    MOTOR_SetSpeed(RIGHT, -0.1);
    MOTOR_SetSpeed(LEFT, 0.1);
    Mg = ENCODER_Read(LEFT);
  }
  while(Mg<(seuilAngle * 0.06))   //augmentation de la vitesse jusqu'a 6% de la rotation
  {
    MOTOR_SetSpeed(RIGHT, -0.2);
    MOTOR_SetSpeed(LEFT, 0.2);
    Mg = ENCODER_Read(LEFT);
  }
  while(Mg<(seuilAngle*0.94))    //vitesse maximale jusqu'a 94% de la rotation
  {
    MOTOR_SetSpeed(RIGHT, -0.3);
    MOTOR_SetSpeed(LEFT, 0.3+0.00535);
    Mg = ENCODER_Read(LEFT);
  }
  while(Md<seuilAngle)        //ralentissement pour les derniers 6% de la rotation
  {
    MOTOR_SetSpeed(RIGHT, -0.1);
    MOTOR_SetSpeed(LEFT, 0.1);
    Md = ENCODER_Read(RIGHT);
    Mg = ENCODER_Read(LEFT);
  }
  //Mg = Mg -1;
  Mg = -1;
  while(Mg-Md >5)   //Tant que la difference entre les deux encodeurs est plus grande que 5 pulses les moteurs sont pas allignés
  {
    while(Md>=Mg)
    {
      MOTOR_SetSpeed(RIGHT, -0.1);
      MOTOR_SetSpeed(LEFT, 0);
      Mg = ENCODER_Read(LEFT)*-1;
      //Mg = ENCODER_Read(LEFT);
    }
    while(Md<=Mg)
    {
      MOTOR_SetSpeed(RIGHT, 0.1);
      MOTOR_SetSpeed(LEFT, 0.1);
      Mg = ENCODER_Read(LEFT)*-1;
      //Mg = ENCODER_Read(LEFT);
    }
  }
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);
  delay(175);
}

bool detectSiflet(){

 int pin35 = digitalRead(sifletAmbient);
 int pin43 = digitalRead(siflet5Khz);
  if(pin35 && pin43){
    return true;
  }
  else {
    return false;
  }
}

void suiveurLigne()
{
  bool sort = false;

  while(sort == false)
  {
    prendreValeurSuiveur();

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
        MOTOR_SetSpeed(RIGHT,0.3);
        MOTOR_SetSpeed(LEFT,0.3);
        delay(1000);
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);

        SERVO_Enable(0);
        SERVO_Enable(1);
        SERVO_SetAngle(0,80);
        SERVO_SetAngle(1,110);

        sort = true;
        delay(500);
        //while(1);
    }
  }
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);
}

//detecte couleur  --> retourne 1 si rouge, 2 si bleu, 3 si vert, 4 si jaune, 0 si aucune couleur
void detectCouleur()
{
  uint16_t r, g, b, c;

  tcs.getRawData(&r, &g, &b, &c);     //lecture des valeurs RGB

  int red = r;
  int green = g;
  int blue = b;

  int rRed = 660;
  int gRed = 620;
  int bRed = 677;

  int rBlue = 406;
  int gBlue = 675;
  int bBlue = 840;

  int rGreen = 425;
  int gGreen = 705;
  int bGreen = 719;

  int rYellow = 1100;
  int gYellow = 1150;
  int bYellow = 845;

  /*int rRed = 630;
  int gRed = 605;
  int bRed = 658;

  int rBlue = 383;
  int gBlue = 632;
  int bBlue = 778;

  int rGreen = 399;
  int gGreen = 665;
  int bGreen = 676;

  int rYellow = 1027;
  int gYellow = 1074;
  int bYellow = 797;*/


  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.println(" ");

  if(abs(red - rRed) < 25 && abs(green - gRed) < 25 && abs(blue - bRed) < 25)
  {
    couleur = 1;
    //return 1;
  }
    //Serial.print("Rouge identifie");
  else if(abs(red - rBlue) <25 && abs(green-gBlue) < 25 && abs(blue - bBlue) < 40)   //pour bleu
    {
      couleur = 2;
      //return 2;
    }
  //Serial.print("Bleu identifie");
  else if(abs(red-rGreen) <35 && abs(green - gGreen) <35 && abs(blue - bGreen) <35)    //pour vert
    {
      //return 3;
      couleur = 3;
    }
    //Serial.print("Vert identifie");
  else if (abs(red-rYellow) < 70 && abs(green - gYellow) < 70 && abs(blue -bYellow) <70)   //pour jaune
    {
      //return 4;
      couleur = 4;
    }
    //Serial.print("Jaune identifie");
  else
  {
    //return 0;
    couleur = 0;
  }
    //Serial.print("Aucune couleur identifie");
}

void premierVirage()
{
  if(couleur == 1)
  {
    //tournerAngleGauche(135);
    tournerAngleGauche(180);
    delay(100);
    scan90Droit();

  }
  else if(couleur == 2)
  {
    //tournerAngleGauche(315);
    scan90Droit();
  }
  else if(couleur == 3)
  {
    //tournerAngleGauche(225);
    tournerAngleGauche(180);
    delay(100);
    scan90Gauche();
  }
  else if(couleur == 4)
  {
    //tournerAngleGauche(45);
    scan90Gauche();
  }
  /*else
  {
    tournerAngleGauche(360);
  }*/
  MOTOR_SetSpeed(LEFT,0);
  MOTOR_SetSpeed(RIGHT,0);
}


void tournantSuite(int nbOjetsButs)
{
  if (nbObjetsButs == 1)
  {
    tournerAngleDroit(135);
  }
  else if (nbObjetsButs == 2)
  {
    tournerAngleDroit(45);
  }
  else if (nbObjetsButs == 3)
  {
    tournerAngleGauche(45);
  }
}

//recule en demi lune et lacher objet
void deposerObjet()
{

}

void allerCentreSuite(int nbObjetsButs)
{

}

void allerCentreDebut()
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
        lignedetecte = 1;
        suiveurLigne();
        break;
      }
    }

    if(lignedetecte == 0)
    {
      MOTOR_SetSpeed(LEFT,0.2);
      MOTOR_SetSpeed(RIGHT,0.2);
      delay(300);
    }
    else{
      MOTOR_SetSpeed(LEFT, 0);
      MOTOR_SetSpeed(RIGHT, 0);
      break;
      //break;
    }
    if(lignedetecte == 0)
    {
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
          lignedetecte = 1;
          suiveurLigne();
          break;
        }
      }
    }
    if(lignedetecte == 0)
    {
      MOTOR_SetSpeed(LEFT,0.2);
      MOTOR_SetSpeed(RIGHT,0.2);
      delay(300);
    }
    else{
      MOTOR_SetSpeed(LEFT, 0);
      MOTOR_SetSpeed(RIGHT, 0);
      break;
      break;
    }
  }
  MOTOR_SetSpeed(LEFT,0);
  MOTOR_SetSpeed(RIGHT,0);
}

// change la valeur de capt1, capt2, capt3 pour les valeurs lues
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

void touverLigneExtremite()      // a retravailler cas ou les 3 capteurs sont sur la ligne   *****************************************************************************
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
      suiveurLigne();
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

      MOTOR_SetSpeed(RIGHT,0);

      suiveurLigne();
      sortie = true;
    }
    else if(capt3 == 1)
    {
      //while(1);
      suiveurLigne();
      sortie = true;
    }
  }
}


int hasobject = 0;
void PrendreObjet()
{
  int distance_cm;
  SharpIR mySensor = SharpIR(IRPin, model);
  distance_cm = mySensor.distance();
  //delay(5);
  while(distance_cm <= 10)
  {
    MOTOR_SetSpeed(RIGHT,0.3);
    MOTOR_SetSpeed(LEFT,0.3);

    SharpIR mySensor = SharpIR(IRPin, model);
    distance_cm = mySensor.distance();
    delay(5);
  }

  // Detection d'objet
  while (distance_cm <= 10 && distance_cm > 6)
  {
    //Fermer les bras quand l'objet est a 7cm

    //SERVO_Enable(0);
    //SERVO_Enable(1);
    hasobject = 1;

    MOTOR_SetSpeed(RIGHT,0.15);
    MOTOR_SetSpeed(LEFT,0.15);
    delay(1000);

    delay(500);
    ferme_bras();

    delay(500);
    SERVO_Disable(0);
    SERVO_Disable(1);

    touverLigneExtremite();
  }
}

void TestTestTest()     //code pour tester fonctionnement du capt1 suiveur le ligne   ----> fonctionne
{
   while(1)
   {
    prendreValeurSuiveur();

    MOTOR_SetSpeed(LEFT,0.15);
    MOTOR_SetSpeed(RIGHT,0.15);

    if(capt1 == 1)
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

      MOTOR_SetSpeed(RIGHT,0);

      suiveurLigne();
    }
   }
}

void V2()
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

      MOTOR_SetSpeed(RIGHT,0);

      suiveurLigne();
      sortie = true;
    }
    else if(capt3 == 1)
    {
      //while(1);
      suiveurLigne();
      sortie = true;
    }
  }
}

void scan90Gauche()
{
  int Md = 0;
  int Mg = 0;
  int seuilAngle = 1905;

  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);

  while(Md<seuilAngle)
  {

    getdistance();
    MOTOR_SetSpeed(RIGHT, 0.05);
    MOTOR_SetSpeed(LEFT, -0.05);
    Md = ENCODER_Read(RIGHT);
    Mg = ENCODER_Read(LEFT);
    getdistance();
    //si detecte objet arrete de tourner et avance chercher l'objet
    if(distance_cm<37)
    {
      MOTOR_SetSpeed(RIGHT,0);
      MOTOR_SetSpeed(LEFT,0);
      PrendreObjet();
      break;
    }
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

void scan90Droit()
{
int Md = 0;
  int Mg = 0;
  int seuilAngle = 1905;

  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);

  while(Md<seuilAngle)
  {

    MOTOR_SetSpeed(LEFT, 0.05);
    MOTOR_SetSpeed(RIGHT, -0.05);
    Md = ENCODER_Read(LEFT);
    Mg = ENCODER_Read(RIGHT);
    getdistance();
    //si detecte objet arrete de tourner et avance chercher l'objet
    if(distance_cm<37)
    {
      MOTOR_SetSpeed(RIGHT,0);
      MOTOR_SetSpeed(LEFT,0);
      PrendreObjet();

    }
    //si detecte objet arrete de tourner et avance chercher l'objet
  }
  //Mg = Mg -1;
  Mg = -1;
  while(Mg-Md >5)   //Tant que la difference entre les deux encodeurs est plus grande que 5 pulses les moteurs sont pas allignés
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
  }
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);
  delay(175);
}