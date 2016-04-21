/*
name                    TANVIR SAJED
ONE Card number         1263382
Unix id                 tsajed
lecture section         A1
instructor's name       Martin Mueller
lab section             D05
TA's name               Van hai Ho

*/


//functions.h contains all the function prototypes that are
//required by the normal version of the game


int Call_LeftKey(landerdata *);

int Call_RightKey(landerdata *);

int Call_Thrust(landerdata *);

int InitLander(landerdata *);

int rotate(landerdata *, int , double);

int ListenKey(landerdata *);

int UpdateLander(landerdata *);

int Intersection(landerdata *, int, int);

int Find_MinMax(landerdata *, int, int);

int Find_IPoint(landerdata *, int, int, double);

int CheckLanding(landerdata *);

void DrawThrust(landerdata *);
