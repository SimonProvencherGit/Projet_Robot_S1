#include <Arduino.h>
#include <LibRobus.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

//declaration des fonctions
void tournerAngleDroit(int angle);
void tournerAngleGauche(int angle);
bool detectSiflet();


// Déclaration des variables globales

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

int sifletAmbient = 35;
int siflet5Khz = 43;
bool depart = false;
bool bumperArriere;
uint8_t id = 3;
int vitesse = 1;
String couleur[5] = {"Aucune","Rouge","Bleu","Vert","Jaune"};


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

  bumperArriere = ROBUS_IsBumper(id);

  if(bumperArriere)
      depart = true;

  if(depart) {
    couleur = detectCouleur();
    if(couleur != 0)
    {
      
    } 
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
  int capt1,capt2,capt3;
    
  capt1 = analogRead(A0);            //lecture des capteurs de détecteurs de ligne
  capt2 = analogRead(A1);
  capt3 = analogRead(A2);

  if(capt1<500)
      capt1 = 0;
  else if(capt1>=500)
      capt1 = 1;
  
  if(capt2<100)
      capt2 = 0;
  else if(capt2>=100)
      capt2 = 1;  
  
  if(capt3<100)
      capt3 = 0;
  else if(capt3>=100)
      capt3 = 1;
  
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
  }
}

//fonction pour detecter la couleur
//retourne 1 si rouge, 2 si bleu, 3 si vert, 4 si jaune, 0 si aucune couleur
int detectCouleur()
{
  uint16_t r, g, b, c;

  tcs.getRawData(&r, &g, &b, &c);     //lecture des valeurs RGB

  int red = r;
  int green = g;
  int blue = b;

  if(red>740 && green<815 && blue<705)    //verifie les valeurs rgb pour rouge
    return 1;
    //Serial.print("Rouge identifie");
  else if(red<650 && green>760 && blue>670)   //pour bleu
    return 2;
  //Serial.print("Bleu identifie");
  else if(red<630 && green>800 && blue<750)    //pour vert
    return 3;
    //Serial.print("Vert identifie");
  else if (red>700 && green>850 && blue<760)   //pour jaune
    return 4;  
    //Serial.print("Jaune identifie");
  else
    return 0;
    //Serial.print("Aucune couleur identifie");
}


