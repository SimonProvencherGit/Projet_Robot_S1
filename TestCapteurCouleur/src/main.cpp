#include <Arduino.h>
#include <LibRobus.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup() 
{
    Serial.begin(9600);
    Serial.println("Color View Test!");

    pinMode(A0,INPUT);              //initialisation des capteurs de détecteurs de ligne
    pinMode(A1,INPUT);
    pinMode(A2,INPUT);

    if (tcs.begin()) {
        Serial.println("Found sensor");
    } else {
        Serial.println("No TCS34725 found ... check your connections");
        while (1); // halt!
    }
}


void loop(void) {
    uint16_t r, g, b, c, colorTemp, lux;
    bool capt1,capt2,capt3;


    tcs.getRawData(&r, &g, &b, &c);     //lecture des valeurs RGB

    //colorTemp = tcs.calculateColorTemperature(r, g, b);
    //lux = tcs.calculateLux(r, g, b);

    //Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
    //Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");

    capt1 = digitalRead(A0);            //lecture des capteurs de détecteurs de ligne
    capt2 = digitalRead(A1);
    capt3 = digitalRead(A2);

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

    if(red>705 && green<815 && blue<705)    //verifie les valeurs rgb pour rouge
        Serial.print("Rouge identifie");
    else if(red<750 && green>850 && blue>850)   //pour bleue
        Serial.print("Bleu identifie");
    else if(red<630 && green>800 && blue<750)    //pour vert
        Serial.print("Vert identifie");
    else if (red>700 && green>850 && blue<760)   //pour jaune
        Serial.print("Jaune identifie");
    else
        Serial.print("Aucune couleur identifie");
    
}
