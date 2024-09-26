
/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

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
float vitesse = 0.40;
bool depart = false;

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
  MOTOR_SetSpeed(RIGHT,vitesse);
  MOTOR_SetSpeed(LEFT, vitesse);
};

void recule(){
  MOTOR_SetSpeed(RIGHT, -0.5*vitesse);
  MOTOR_SetSpeed(LEFT, -vitesse);
};

void tourneDroit(){
  MOTOR_SetSpeed(RIGHT, 0.5*vitesse);
  MOTOR_SetSpeed(LEFT, -0.5*vitesse);
};

void tourneGauche(){
  MOTOR_SetSpeed(RIGHT, -0.5*vitesse);
  MOTOR_SetSpeed(LEFT, 0.5*vitesse);
};

bool detectSiflet(){
  return true;
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
  tourneDroit();
  delay(10);
  tourneDroit();
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
      tourneDroit();
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
  }
}
