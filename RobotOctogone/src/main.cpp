#include <Arduino.h>
#include <LibRobus.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

//declaration des fonctions
void tournerAngleDroit(int angle);
void tournerAngleGauche(int angle);
bool detectSiflet();
void premierVirage(int couleur);
void allerCentreDebut();
void touverLigneExtremite();
void prendreValeurSuiveur();
int detectCouleur();
void suiveurLigne();
void TestTestTest();
void V2();
void tournantSuite(int nbOjetsButs);

// Déclaration des variables globales

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

int sifletAmbient = 35;
int siflet5Khz = 43;
bool depart = false;
bool bumperArriere;
uint8_t id = 3;
int vitesse = 1;
String couleur[5] = {"Aucune","Rouge","Bleu","Vert","Jaune"};
int nbObjetsButs = 0;
int capt1,capt2,capt3;

void setup() {
  BoardInit();
  Serial.begin(9600);

  pinMode(A0,INPUT);              //initialisation des capteurs de détecteurs de ligne
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);

  if (tcs.begin()) {
        Serial.println("Found sensor");
    } else {
        Serial.println("No TCS34725 found ... check your connections");
       // while (1); // halt!
    }


}

void loop() {
  int couleur;
  bool sortie = false;
  
  while(sortie == false)
  {
    for(int i=0; i<50;i++)
    {
      MOTOR_SetSpeed(LEFT, 0.3);
      MOTOR_SetSpeed(RIGHT, 0.15);
      prendreValeurSuiveur();
      if(capt1 ==1 || capt3 == 1)
      {
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        sortie = true;
        suiveurLigne();
      }
    }
    for(int i=0; i<50;i++)
    {
      MOTOR_SetSpeed(LEFT, 0.15);
      MOTOR_SetSpeed(RIGHT, 0.3);
      prendreValeurSuiveur();
      if(capt1 ==1 || capt3 == 1)
      {
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        sortie = true;
        suiveurLigne();
      }
    }
      while (1);
  }  
  //V2();
  //touverLigneExtremite();

  bumperArriere = ROBUS_IsBumper(id);

  if(bumperArriere)
      depart = true;

  if(depart) {
    
    couleur = detectCouleur();
    
    if(couleur != 0)
    { 
      //code pour trouver la premiere ligne et se rendre au centre

      if(nbObjetsButs == 0)   //tourner pour faire face au bon triangle
      {
        premierVirage(couleur);
      }
      else
      {
        tournantSuite(nbObjetsButs);      
      }
    }

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
  double seuilAngle1;

  //calculs pour trouver la valeur des encodeurs pour l'angle voulu
  // 1905/90 = x/angle
  // x = 1905/90 * angle  = nb de pulses pour l'angle voulu

  seuilAngle1 = angle * (1905/90); 

  int Md = 0;
  int Mg = 0;
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);
  while(Md<(seuilAngle1*0.03))
  {
    MOTOR_SetSpeed(LEFT, 0.1);
    MOTOR_SetSpeed(RIGHT, -0.1);
    Md = ENCODER_Read(LEFT);
  }
    while(Md<(seuilAngle1 * 0.06))
  {
    MOTOR_SetSpeed(LEFT, 0.2);
    MOTOR_SetSpeed(RIGHT, -0.2);
    Md = ENCODER_Read(LEFT);
  }
  while(Md<(seuilAngle1*0.94))
  {
    MOTOR_SetSpeed(LEFT, 0.3);
    MOTOR_SetSpeed(RIGHT, -0.3-0.00535);
    Md = ENCODER_Read(LEFT);
  }
  while(Md<seuilAngle1)
  {
    MOTOR_SetSpeed(LEFT, 0.1);
    MOTOR_SetSpeed(RIGHT, -0.1);
    Md = ENCODER_Read(LEFT);
    Mg = ENCODER_Read(RIGHT);
  }
  //Mg = Mg -1;
  Mg = -1;
  while(Mg-Md >5)   //Tant que la difference entre les deux encodeurs est plus grande que 5 pulses
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

        sort = true;  
        while(1); 
    }
  }
  MOTOR_SetSpeed(RIGHT,0);
  MOTOR_SetSpeed(LEFT,0);
}

//detecte couleur  --> retourne 1 si rouge, 2 si bleu, 3 si vert, 4 si jaune, 0 si aucune couleur
int detectCouleur()
{
  uint16_t r, g, b, c;

  tcs.getRawData(&r, &g, &b, &c);     //lecture des valeurs RGB

  int red = r;
  int green = g;
  int blue = b;

  int rRed = 865;
  int gRed = 853;
  int bRed = 690;

  int rBlue = 544;
  int gBlue = 863;
  int bBlue = 769;

  int rGreen = 570;
  int gGreen = 932;
  int bGreen = 697;

  int rYellow = 1343;
  int gYellow = 1516;
  int bYellow = 901;


  /*Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");*/

  if(red - rRed < 50 && green - gRed < 50 && blue - bRed < 50)    
    return 1;
    //Serial.print("Rouge identifie");
  else if(red - rBlue && green-gBlue < 50 && blue - bBlue < 50)   //pour bleu
    return 2;
  //Serial.print("Bleu identifie");
  else if(red-rGreen <50 && green - gGreen <50 && blue - bGreen <50)    //pour vert
    return 3;
    //Serial.print("Vert identifie");
  else if (red-rYellow < 70 && green - gYellow < 70 && blue -bYellow <70)   //pour jaune
    return 4;  
    //Serial.print("Jaune identifie");
  else
    return 0;
    //Serial.print("Aucune couleur identifie");
}

void premierVirage(int couleur)
{
  if(couleur == 1)
  {
    tournerAngleGauche(135);
  }
  else if(couleur == 2)
  {
    tournerAngleDroit(135);
  }
  else if(couleur == 3)
  {
    tournerAngleGauche(45);
  }
  else if(couleur == 4)
  {
    tournerAngleDroit(45);
  }
  /*else
  {
    tournerAngleGauche(360);
  }*/
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

//recule en demi lune pour lacher objet
void deposerObjet()
{

}

void allerCentreDebut(int couleur)
{
  bool sortie = false;

  while(couleur != 0)
  {
    MOTOR_SetSpeed(LEFT, 0.2);
    MOTOR_SetSpeed(RIGHT, 0.2);
    couleur = detectCouleur();
  }
  while(sortie == false)
  {
    for(int i=0; i<400;i++)
    {
      MOTOR_SetSpeed(LEFT, 0.3);
      MOTOR_SetSpeed(RIGHT, 0.15);
      prendreValeurSuiveur();
      if(capt1 ==1 || capt3 == 1)
      {
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        sortie = true;
      }
    }
    for(int i=0; i<400;i++)
    {
      MOTOR_SetSpeed(LEFT, 0.15);
      MOTOR_SetSpeed(RIGHT, 0.3);
      prendreValeurSuiveur();
      if(capt1 ==1 || capt3 == 1)
      {
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        sortie = true;
      }
    }

    MOTOR_SetSpeed(LEFT,0.3);
    MOTOR_SetSpeed(RIGHT,0);

    prendreValeurSuiveur();


    
  }
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
  else if(capt2>30)
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

void touverLigneExtremite()
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