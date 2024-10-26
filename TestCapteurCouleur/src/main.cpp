#include <Arduino.h>
#include <LibRobus.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

int sifletAmbient = 35;
int siflet5Khz = 43;
bool depart = false;
bool bumperArriere;

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


void setup() 
{
    BoardInit();
    
    Serial.begin(9600);
    //Serial.println("Color View Test!");

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


void loop(void) 
{
    int capt1,capt2,capt3;
    
    int vitesse = 1;
    uint8_t id = 3;

    /*if(detectSiflet())
    {
        depart = true;
        Serial.print("siflet detecte");
    }*/
    /*capt1 = analogRead(A0);            //lecture des capteurs de détecteurs de ligne
    capt2 = analogRead(A1);
    capt3 = analogRead(A2);
    
    if(capt1<800)
            capt1 = 0;
        else if(capt1>800)
            capt1 = 1;
        
        if(capt2<100)
            capt2 = 0;
        else if(capt2>100)
            capt2 = 1;  
        
        if(capt3<100)
            capt3 = 0;
        else if(capt3>100)
            capt3 = 1;
    
    Serial.print(capt1,DEC);        //affiche les valeurs des capteurs de détecteurs de ligne
    Serial.print("   "); 
    Serial.print(capt2,DEC);
    Serial.print("   ");
    Serial.print(capt3,DEC);
    Serial.println("   ");    
    
    delay(500);*/
    bumperArriere = ROBUS_IsBumper(id);

    if(bumperArriere)
        depart = true;

    if(depart)
    {   
        capt1 = analogRead(A0);            //lecture des capteurs de détecteurs de ligne
        capt2 = analogRead(A1);
        capt3 = analogRead(A2);

        if(capt1<800)
            capt1 = 0;
        else if(capt1>800)
            capt1 = 1;
        
        if(capt2<100)
            capt2 = 0;
        else if(capt2>100)
            capt2 = 1;  
        
        if(capt3<100)
            capt3 = 0;
        else if(capt3>100)
            capt3 = 1;
        
        if(capt1 == 0 && capt2 == 1 && capt3 == 0)
        {
            MOTOR_SetSpeed(RIGHT,0.3*vitesse);
            MOTOR_SetSpeed(LEFT,0.3*vitesse);
        }
        else if (capt1 == 1 && capt2 == 1 && capt3 == 0 || capt1 == 1 && capt2 == 0 && capt3 == 0)
        {
            MOTOR_SetSpeed(RIGHT,0.3*vitesse);
            MOTOR_SetSpeed(LEFT,0);
        }
        else if (capt1 == 0 && capt2 == 1 && capt3 == 1 || capt1 == 0 && capt2 == 0 && capt3 == 1)
        {
            MOTOR_SetSpeed(RIGHT,0);
            MOTOR_SetSpeed(LEFT,0.3*vitesse);
        }
        else if(capt1 == 1 && capt2 == 1 && capt3 == 1)
        {
            MOTOR_SetSpeed(RIGHT,0);
            MOTOR_SetSpeed(LEFT,0);
        }
        /*else if(capt1 == 0 && capt2 == 0 && capt3 == 0)
        {
            MOTOR_SetSpeed(RIGHT,0);
            MOTOR_SetSpeed(LEFT,0);
        }
        else{
            MOTOR_SetSpeed(RIGHT,0);
            MOTOR_SetSpeed(LEFT,0);
        }*/
    }
}


    /*uint16_t r, g, b, c, colorTemp, lux;
    bool capt1,capt2,capt3;
    int lim = 100;

    tcs.getRawData(&r, &g, &b, &c);     //lecture des valeurs RGB

    //colorTemp = tcs.calculateColorTemperature(r, g, b);
    //lux = tcs.calculateLux(r, g, b);

    //Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
    //Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");

    capt1 = digitalRead(A7);            //lecture des capteurs de détecteurs de ligne
    capt2 = digitalRead(A3);
    capt3 = digitalRead(A0);

    Serial.print(capt1,DEC);        //affiche les valeurs des capteurs de détecteurs de ligne
    Serial.print(capt2,DEC);
    Serial.print(capt3,DEC);
    Serial.print("   ");

    Serial.print("  R: "); Serial.print(r, DEC); Serial.print(" ");       //affiche les valeurs RGB
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
    Serial.println(" ");

    int red = r;
    int green = g;
    int blue = b;

    if(red>740 && green<815 && blue<705)    //verifie les valeurs rgb pour rouge
        Serial.print("Rouge identifie");
    else if(red<650 && green>760 && blue>670)   //pour bleu
        Serial.print("Bleu identifie");
    else if(red<630 && green>800 && blue<750)    //pour vert
        Serial.print("Vert identifie");
    else if (red>700 && green>850 && blue<760)   //pour jaune
        Serial.print("Jaune identifie");
    else
        Serial.print("Aucune couleur identifie");

    */