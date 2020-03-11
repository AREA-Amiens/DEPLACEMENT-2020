#include <Base Roulante.h>

class AccelStepper;

class Robot
{


public:



    Robot(); // constructeur

    //méthodes

    int go(double distance);

    int turn(double angle,int state);

    void ajoutmoteurDroit(AccelStepper& stepperD);
    void ajoutmoteurGauche(AccelStepper& stepperG);

    void run();

    void stop();

    int fini();

    void move();


private: // Attributs
  AccelStepper* moteurGauche;
  AccelStepper* moteurDroit;
  int passe=1;
  //byte fini=0;


};
