/*
name                    TANVIR SAJED
ONE Card number         1263382
Unix id                 tsajed
lecture section         A1
instructor's name       Martin Mueller
lab section             D05
TA's name               Van hai Ho

*/


//functions_i.h file to contain function prototypes and macros and
//C library macro and functions to be used by the improved version

#define _POSIX_C_SOURCE 200112L
#define MAX_SKETCH 640
#define MIN_SKETCH 0

// must include these 2 header files
#include <signal.h>
#include <sys/time.h>
#include <bits/sigaction.h>

int sigemptyset(sigset_t *);
int sigaddset(sigset_t *, int);
int sigprocmask(int, const sigset_t *, sigset_t *);
int sigaction(int, const struct sigaction *, 
  struct sigaction *);


void SignalBlocker(landerdata *, int);

void InitGauge(landerdata *);

int UpdateGauge(landerdata *);

void End(landerdata *);

landerdata *InitAsteroids(landerdata *);

void UpdateAsteroid(landerdata *, landerdata *);

int AstLandCrash(landerdata *, landerdata *);

int InterAL(landerdata *, landerdata *, int, int);

int Find_ALMinMax(landerdata *, landerdata *, int, int);

int Find_ALPoint(landerdata *, landerdata *, int, int, double);

int random(void);

void Timer(int);

int TranslateX(landerdata *, landerdata *);

double Ast_FindMax(landerdata *, int);

double Ast_FindMin(landerdata *, int);


