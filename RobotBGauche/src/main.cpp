
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
char chemin[30];
int nbAction = 0;

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
  delay(400);
  MOTOR_SetSpeed(RIGHT, -0.20*vitesse);
  MOTOR_SetSpeed(LEFT, 0.20*vitesse);
  delay(975);
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  delay(100);
};

void tourneGauche(){
  delay(400);
  MOTOR_SetSpeed(RIGHT, 0.20*vitesse);
  MOTOR_SetSpeed(LEFT, -0.20*vitesse);
  delay(995);
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  delay(100);
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
  MOTOR_SetSpeed(RIGHT, 0.25 * vitesse);
  MOTOR_SetSpeed(LEFT, -0.25 * vitesse);
  delay(1640);
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  delay(100);
}

/*s'il va vers la droite, il avance de 0.5m et verifie s'il peut avancer jusqu'a ce qu'il puisse avancer*/
void ActionSensDroit()
{
  
  bool progres = false;

  while (progres == false)
  {
    avance();
    posX++;
    chemin[nbAction] = 'D';
    nbAction++;

    tourneGauche();
    delay(100);
    
    if(murDetecte())
    {
      delay(100);
      tourneDroit();
    }
    else
    {
      progres = true;
      avance();
      posY++;
      chemin[nbAction] = 'A';
      nbAction++;
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
  depart = false;

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
            delay(100);
            avance();
            posX--;
            chemin[nbAction] = 'G';
            nbAction++;
            tourneDroit();
            delay(300);
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
        chemin[nbAction] = 'A';
        nbAction++;
      }
    }
    
    faitDemiTour();
    posY = 1;
    //posX = (posX == 0 ? 2 : posX == 2 ? 0 : 1);

    while(posY < 10) 
    {
      for(int i = nbAction; i >= 0; i--)
      {
        if(chemin[i] == 'A')
        {
          posY++; 

          if(chemin[i+1] == 'G')
          {
            tourneDroit();
            //delay(100);
            avance();
          }
          else if(chemin[i+1] == 'D')
          {
            tourneGauche();
            //delay(100);
            avance();
          }
          else
          {
            avance();
          }
        }
        else if(chemin[i] == 'G')
        {
          if(chemin[i+1] == 'A')
          {
            tourneGauche();
            //delay(100);
            avance();
          }
          else if(chemin[i+1] == 'G')
          {
            avance();
          }
          else if(chemin[i+1] == 'D')
          {
            faitDemiTour();
            //delay(100);
            avance();
          }
        }
        else if(chemin[i] == 'D')
        {
          if(chemin[i+1] == 'A')
          {
            tourneDroit();
            //delay(100);
            avance(); 
          }
          else if(chemin[i+1] == 'G')
          {
            faitDemiTour();
            //delay(100);
            avance();
          }
          else if(chemin[i+1] == 'D')
          {
            avance();
          }
        }
      }
    }
  }
}
