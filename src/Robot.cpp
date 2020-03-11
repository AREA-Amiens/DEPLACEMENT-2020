#include <Robot.h>


// Méthodes

Robot :: Robot(){
/*
  moteurDroit->setMaxSpeed(500);
  moteurGauche->setMaxSpeed(500);

  moteurDroit->setAcceleration(100);
  moteurGauche->setAcceleration(100);
*/
}


int Robot::fini(){
  if (moteurGauche->isRunning()==0 && moteurDroit->isRunning()==0){
    return 1;
  }
    //int fini(1)}
  else return 0;
}

int Robot::go(double distance)
{
  if (this->fini()==1){
    moteurDroit->move(distance*7.520210565);
    moteurGauche->move(-distance*7.520210565);
    //passe=0;

  }
  //Serial.print("a");
  /*
  Serial.print("position");
  int i=moteurDroit->currentPosition();
  Serial.print(i);*/
  moteurDroit->run();
  moteurGauche->run();


  if (this->fini()==1)return 1;
  else return 0;
}

int Robot::turn(double angle,int state){
  if(this->fini()==1){
    //Serial.print("7");
    moteurDroit->move(angle*12.99399105*360/(2*PI));
    moteurGauche->move(angle*12.99399105*360/(2*PI));
    //Serial.print("angle");
    //Serial.print(angle*360/(2*PI));
    passe=1;
  }
  //Serial.print(passe);
  //Serial.print(state);
  moteurDroit->run();
  moteurGauche->run();

  int step_done=moteurDroit->currentPosition();
  //Serial.print("PASFAIT");
  //Serial.println(step_done);


  if (this->fini()==1){passe=0;return 1;}
  else return 0;
}

void Robot::run(){
    moteurDroit->run();
    moteurGauche->run();
}

void Robot::stop(){
  moteurDroit->stop();
  moteurGauche->stop();
}


void Robot::ajoutmoteurDroit(AccelStepper& stepperD){
  moteurDroit= &stepperD;
}

void Robot::ajoutmoteurGauche(AccelStepper &stepperG){
  moteurGauche= &stepperG;
}
