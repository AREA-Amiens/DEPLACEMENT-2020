#include <arduino.h>
#include <AccelStepper.h>
#include <math.h>
#define pi 3.1415926536
//#include <MsTimer2.h>


#include <Wire.h>
//Cette bibliothèque vous permet de communiquer avec les led RGB de chez Grove

// Définition de l'adresse logique de cet esclave I2C.
#define I2C_SLAVE_ADDRESS 2

#define RESET_G   15
#define SLEEP_G   13
#define STEP_G    0
#define DIR_G     1

#define RESET_D 9
#define SLEEP_D 10
#define STEP_D 11
#define DIR_D 12

void manageEvent(int howMany);

void requestEvent();
