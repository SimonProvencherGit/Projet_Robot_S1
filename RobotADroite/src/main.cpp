
/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

//////////////////////////   ROBOT B ////////////////////////////////////////////

/*
Inclure les librairies de functions que vous voulez utiliser
*/
#include <LibRobus.h>
#include <Arduino.h>

/*
Variables globales et defines
 -> defines...
 -> L'ensemble des fonctions y ont acces
*/

bool bumperArr;
int vertpin = 23;
int rougepin = 22;
bool vert = false;
bool rouge = false;
int etat = 0; // = 0 arrêt 1 = avance 2 = recule 3 = TourneDroit 4 = TourneGauche
int etatPast = 0;
float vitesse = 1;
bool depart = false;
int sifletAmbient = 35;
int siflet5Khz = 43;

int posX = 1;
int posY = 1;

/*
Vos propres fonctions sont creees ici
*/

void beep(int count){
  for(int i=0;i<count;i++){
    AX_BuzzerON();
    delay(100);
    AX_BuzzerOFF();
    delay(100);  
  }
  delay(400);
}

void arret(){
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
};

void avance(){
  MOTOR_SetSpeed(RIGHT,0.5*vitesse);
  MOTOR_SetSpeed(LEFT, 0.5*vitesse);
  delay(1320);
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

};

void recule(){
  MOTOR_SetSpeed(RIGHT, -0.5*vitesse);
  MOTOR_SetSpeed(LEFT, -vitesse);
};

void tourneDroit(){
  delay(500);
  MOTOR_SetSpeed(RIGHT, -0.25*vitesse);
  MOTOR_SetSpeed(LEFT, 0.25*vitesse);
  delay(720);
};

void tourneGauche(){
  delay(500);
  MOTOR_SetSpeed(RIGHT, 0.25*vitesse);
  MOTOR_SetSpeed(LEFT, -0.25*vitesse);
  delay(720);
};

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

bool murDetecte(){
  vert = digitalRead(vertpin);
  rouge = digitalRead(rougepin);

   if (vert && rouge){ // aucun obstacle => avance
      return false;
    }
    if (!vert && !rouge){  // obstacle devant => recule
      return true;
    }
}
void faitDemiTour()
{
  delay(100);
  MOTOR_SetSpeed(RIGHT, 0.25);
  MOTOR_SetSpeed(LEFT, -0.25);
  delay(1550);
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
}

/*s'il va vers la droite, il avance de 0.5m et verifie s'il peut avancer jusqu'a ce qu'il puisse avancer*/
void ActionSensDroit()
{
  
  bool progres = false;

  while (progres == false)
  {
    avance();
    posX++;

    tourneGauche();
    
    if(murDetecte())
    {
      delay(200);
      tourneDroit();
    }
    else
    {
      progres = true;
      avance();
      posY++;
    }
  }
}

/*
Fonctions d'initialisation (setup)
 -> Se fait appeler au debut du programme
 -> Se fait appeler seulement un fois
 -> Generalement on y initilise les varibbles globales
*/
void setup(){
  BoardInit();
  
  //initialisation
  pinMode(vertpin, INPUT);
  pinMode(rougepin, INPUT);
  delay(100);
  beep(3);
}

/*
Fonctions de boucle infini
 -> Se fait appeler perpetuellement suite au "setup"
*/
void loop() 
{
  //avance();
  //tourneDroit();
  //avance();
  //tourneGauche();
  //avance();
 // faitDemiTour();
 //ActionSensDroit();

 /* while (true)
  {
    delay(100);
  }*/

  if(detectSiflet())
    depart = true;
  
  if(depart)
  {
    while(posY < 10) //si on est pas a la fin du labyrinthe
    {
      if(murDetecte())
      {
        if(posX != 0) //si on est pas sur l'extrémité gauche
        {
          tourneGauche();
          
          if(murDetecte()) //si mur devant et mur a gauche
          {
            faitDemiTour();
            
            ActionSensDroit();
          }
          else  //mur devant et pas de mur a gauche
          {
            avance();
            posX--;
            tourneDroit();
          }
        }
        else  //si on est sur l'extrémité gauche
        {
          tourneDroit();
          
          ActionSensDroit();
        }
      }
      else  //si on peut avancer
      {
        avance();
        posY++;
      }
    }
    
    faitDemiTour();
    posY = 1;
    posX = (posX == 0 ? 2 : posX == 2 ? 0 : 1);
  }
}
