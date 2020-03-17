#include <Base Roulante.h>
#include <Robot.h>

AccelStepper moteurGauche(1, STEP_G, DIR_G);//declaration du moteur gauche
AccelStepper moteurDroit(1, STEP_D, DIR_D);//declatation du moteur droit

Robot grandRobot;

byte etat=1, etatp=0,reception_tram[73],com=0,stop=0;
int i=0,go=0, turndepart=0,turnarrive=0,turnar,turnactu=0,etatD=0,a=0,b=0;

double angletraj=0,angleactu=0,angleTurn1=0,angleTurn2=0,anglevoulu=0, Dan=0,deplacement[18][3];


bool receivedI2CSignal = false;


void setup() {
  /*
  //position initiale
  deplacement[0][1]=1000;//x
  deplacement[0][2]=1000;//y
  deplacement[0][3]=0;//angle initial

  deplacement[1][1]=750;
  deplacement[1][2]=1000;
  deplacement[1][3]=PI/6;//angle voulu

  deplacement[2][1]=1000;
  deplacement[2][2]=1000;
  deplacement[2][3]=PI/6;

  deplacement[3][1]=750;
  deplacement[3][2]=750;
  deplacement[3][3]=0;

  deplacement[4][1]=1000;
  deplacement[4][2]=1000;
  deplacement[4][3]=PI/3;

  deplacement[5][1]=1250;
  deplacement[5][2]=750;
  deplacement[5][3]=0;

  deplacement[6][1]=1000;
  deplacement[6][2]=1000;
  deplacement[6][3]=0 ;

  deplacement[7][1]=1250;
  deplacement[7][2]=1250;
  deplacement[7][3]=0;

  deplacement[8][1]=1000;
  deplacement[8][2]=1000;
  deplacement[8][3]=3*PI/2 ;

  deplacement[9][1]=1000;
  deplacement[9][2]=750;
  deplacement[9][3]=PI/2;

  deplacement[10][1]=1000;
  deplacement[10][2]=1000;
  deplacement[10][3]=0;

  deplacement[11][1]=750;
  deplacement[11][2]=1250;
  deplacement[11][3]=3*PI/2;

  deplacement[12][1]=1000;
  deplacement[12][2]=1000;
  deplacement[12][3]=0;

  deplacement[13][1]=1250;
  deplacement[13][2]=1000;
  deplacement[13][3]=PI/4;

  deplacement[14][1]=1000;
  deplacement[14][2]=1000;
  deplacement[14][3]=0;

  deplacement[15][1]=1000;
  deplacement[15][2]=1250;
  deplacement[15][3]=0;

  deplacement[16][1]=1000;
  deplacement[16][2]=1000;
  deplacement[16][3]=0;

  deplacement[17][1]=1000;
  deplacement[17][2]=1000;
  deplacement[17][3]=0;*/



  // Ouvre le port série et fixe le debit de communication à 9600 bauds
 Serial.begin(9600);
  // Initialise la library Wire et se connecte au bus I2C en tant que maître
 Wire.begin(I2C_SLAVE_ADDRESS);



Wire.onReceive(manageEvent);//donne la fonction a ouvrire l'or de la réception de dune trasmition

Wire.onRequest(requestEvent);//va envoyer se qui se trouve dans la fonciton

 // Définition de la fonction qui prendra en charge les informations reçues sur le bus I2C.
 //if (Wire.available() > 0) {
   /*//Serial.print("HELLO");
   // Enregistre le fait qu'au moins une valeur a été reçue sur le bus I2C, ce qui désactive
   // totalement l'effet du bouton.
   receivedI2CSignal = true;

   // Lit la donnée suivante, disponible sur le bus I2C.
   for (a=0; a<=17; a=a+1){
     for (b=0; b<=3; b=b+1){
       //Serial.print("HELLO");
       deplacement[a][b]= Wire.read();
     }
   }*/
 //}

 // nb
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);

  pinMode(RESET_G, OUTPUT);    //le reset se fait a l'état bas
  digitalWrite(RESET_G, HIGH);
  pinMode(SLEEP_G, OUTPUT);    //le sleep se met a l'état bas pour une carte fonctionelle
  digitalWrite(SLEEP_G, HIGH);

  //initialisation du //moteur droit
  pinMode(RESET_D, OUTPUT);    //le reset se fait a l'état bas
  digitalWrite(RESET_D, HIGH);
  pinMode(SLEEP_D, OUTPUT);    //le sleep se met a l'état bas pour une carte fonctionelle
  digitalWrite(SLEEP_D, HIGH);


  moteurDroit.setMaxSpeed(10000);
  moteurGauche.setMaxSpeed(10000);

  moteurDroit.setAcceleration(5000);
  moteurGauche.setAcceleration(5000);

  grandRobot.ajoutmoteurDroit(moteurDroit);
  grandRobot.ajoutmoteurGauche(moteurGauche);

  delay(10000);
}



void loop(){
  if (stop==1){ //Si la Stratégie demande à s'arreter (car détection)
    etat=0; // alors renvoi à l'état d'arret
  }

  switch (etat){

    case 0: // Le robot doit s'arreter
    grandRobot.stop();
    break;

    case 1: // Algo pour trouver le turn et le go

    //Serial.println("___________________");
    //Serial.println(i);
    //Serial.println("_______");

    com=1; //le robot va se mouvoir
    i+=1;

    anglevoulu=deplacement[i][3]/100;
    //Serial.println(anglevoulu);

    // Calcul GO
    go = (float)sqrt((double)pow((deplacement[i][1]-deplacement[i-1][1]),2)+pow((deplacement[i][2]-deplacement[i-1][2]),2));

    // Calcul ANGLE DE TRAJECTOIRE
    if (deplacement[i][1]-deplacement[i-1][1]!=0){
      angletraj=atan((deplacement[i][2]-deplacement[i-1][2])/(deplacement[i][1]-deplacement[i-1][1]));
      if (angletraj<0) angletraj+=2*PI;
      if (deplacement[i][1]-deplacement[i-1][1]<0) angletraj+=PI;
      ////Serial.print("angleTraj");
      ////Serial.print(angletraj*360/(2*PI));
    }
    else {
      if (deplacement[i][2]-deplacement[i-1][2]>0) angletraj=PI/2;
      else angletraj=3*PI/2;

    }
    //Serial.print("angleTurn");
    //Serial.print(angleTurn2*360/(2*PI));
    //Serial.print("angletraj");
    //Serial.println(angleTurn1*360/(2*PI));


    angleTurn1=angletraj-angleactu+2*PI; // calcul angle pour avoir la bonne trajctoire à partir de l'angle actuel
  //  angleTurn1=angleTurn1%(2*PI);

    //angleTurn2=angleTurn2%(2*PI);


    //Serial.print("angletraj");
    //Serial.println(angleTurn1*360/(2*PI));

    ////Serial.print("angleTurn");
    ////Serial.print(angleTurn2*360/(2*PI));

    while(angleTurn1>=2*PI || angleTurn1<=-2*PI){
      if (angleTurn1>0)angleTurn1-=2*PI;
      else angleTurn1+=2*PI;
    }


    //Serial.print("angletraj");
    //Serial.println(angleTurn1*360/(2*PI));



    // Optimisation rotation ( sens trigo--> sens horaire)
    if(angleTurn1>=PI || angleTurn1<=-PI){
      if (angleTurn1>0)angleTurn1-=2*PI;
      else angleTurn1+=2*PI;
    }


    //Serial.print("angletraj");
    //Serial.print(angleTurn1*360/(2*PI));
    //Serial.print("go");
    //Serial.println(go);


    if(angleTurn1>=PI/2 || angleTurn1<=-PI/2){
      if (angleTurn1>0)angleTurn1-=PI;//-angleTurn1;
      else angleTurn1=PI+angleTurn1;
      //angleTurn2+=PI;
      go*=-1;
    }

    //Serial.print("go");
    //Serial.println(go);

    angleTurn2=anglevoulu-angleTurn1-angleactu;//-angleactu; // calcul angle pour pouvoir tourner vers un angle spécial désiré

    //Serial.print("angleTurn");
    //Serial.println(angleTurn2*360/(2*PI));

    while(angleTurn2>=2*PI || angleTurn2<=-2*PI){
      if (angleTurn2>0)angleTurn2-=2*PI;
      else angleTurn2+=2*PI;
    }

    //Serial.print("angleTurn");
    //Serial.println(angleTurn2*360/(2*PI));

    if(angleTurn2>=PI || angleTurn2<=-PI){
      if (angleTurn2>0)angleTurn2-=2*PI;
      else angleTurn2+=2*PI;
      //if (abs(angleTurn2)>=PI/2)go*=-1;
    }
    //Serial.print("angleTurn");
    //Serial.print(angleTurn2*360/(2*PI));
    //Serial.print("angletraj");
    //Serial.print(angleTurn1*360/(2*PI));
    //Serial.print("go");
    //Serial.println(go);


  //  if (angleTurn1>PI) angleTurn1-=2*PI;
  //  if (angleTurn2>PI) angleTurn2-=2*PI;

    //if (angleTurn2>PI)angleTurn2-=360;

    if (angleTurn1!=0 || go!=0 || angleTurn2!=0)etat=2;

    ////Serial.print("TUrn1");
    ////Serial.print(angleTurn1*360/(2*PI));

    if(i==17)etat=0;

    break;

    case 2://Premier turn pour aller à la position suivante

    grandRobot.turn(angleTurn1,etat);
    if(grandRobot.fini()==1){//Quand c'est fini
      etat=3;//Passe à l'état Go
      etatp=2;//Mémorise l'état auquel il sort
    }
    break;

    case 3://Go
    //Serial.print(etat);
    grandRobot.go(go);
    if(grandRobot.fini()==1){
      etat=4;//Passe à l'état Turn
      etatp=3;//Mémorise l'état auquel il sort
    }

    break;

    case 4://Turn pour être à l'angle voulu final pour diverses actions à faire
    grandRobot.turn(angleTurn2,etat);
    if(grandRobot.fini()==1){
      etat=1;//Retourne à l'algo du début
      etatp=4;//Mémorise l'état auquel il sort
      angleactu=anglevoulu;//Actualise l'angle auquel il se trouve à la fin du déplacement
      com=0; //envoi que le robot a fini le déplacement
      delay(2000);
    }

    break;

    }
  }

// Lit la donnée disponible sur le bus I2C.
//howMany représente le nombre de trames/octets arrivant à cette adresse
void manageEvent(int howMany) {

    //Si il n'y a qu'une trame
    //C'est qu'il s'agit d'une trame disant qu'il y a eu détection d'obstacle
    //Le robot doit donc s'arreter
    if (howMany<=1){
      stop=Wire.read();//envoie 1 à stop pour dire de s'arreter
    }

    //Sinon c'est qu'il s'agit d'une position à lire
    else{
      for (int k=0; k<howMany; k++){
        reception_tram[k]= Wire.read();
      }

      //Conversion des bits des trames en valeurs numériques
      for (int j=0; j<howMany/4;j++){
        deplacement[i][1]=reception_tram[4*i+1]+((reception_tram[4*i]&=(0x70))<<4);
        deplacement[i][2]=reception_tram[4*i+2]+((reception_tram[4*i]&=(0x0F))<<8);
        deplacement[i][3]=reception_tram[4*i+3]+((reception_tram[4*i]&=(0x80))<<1);
        Serial.println(deplacement[i][1]);
        Serial.println(deplacement[i][2]);
        Serial.println(deplacement[i][3]);
      }
    }
 }

void requestEvent(){//Renvoie à la carte Stratégie si il est arrivé à la position ou non
  Wire.write(com);
}
