/*
name                    TANVIR SAJED
ONE Card number         1263382
Unix id                 tsajed
lecture section         A1
instructor's name       Martin Mueller
lab section             D05
TA's name               Van hai Ho

*/


//struct.h contains all the external C libraries included, and some function
//prototypes used by both versions of game. it has the main struct
//definition.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <math.h>

#define PI acos(-1.0)

FILE *popen(const char*, const char*);
int pclose(FILE *);

typedef struct landerdata {
    
    char *args[7];
    FILE *p;
    FILE *f;
    long m_coord[256];
    double gravity, thrust;
    double l_coord[256];
    double xV, yV, angle;
    double centre_x, centre_y;
    int fg1, fg2, fgc;
    
} landerdata;


FILE *Check(landerdata *, FILE *);

int InitMap(landerdata *);

double FindCentre(landerdata *);

long int lround(double);

int InitTimer(landerdata *);

FILE *Check2(landerdata *, FILE *);




