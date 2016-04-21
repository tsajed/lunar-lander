/* 
 * name:               Tully Johnson
 * ONE Card number:    1113976
 *
 * Unix id:            tullyj
 * lecture section:    A1/EA1
 * instructor's name:  Martin Muller
 * lab section:        D05
 * TA's name:          Van-Hai
 *
 * Header file containing libraries used in program, links to other header 
 * modules and constant variable used in main.
 */

#ifndef LANDER_H
#define LANDER_H

/* Libraries and other header files used */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "curses.h"
#include "signal.h"
#include "sys/time.h"
#include "bits/sigaction.h"
#include "stdbool.h"
#include "ctype.h"
#include "math.h"

/* Macros and variable declarations. */
/* The expected maximum and minimum buffer size for reading lines of text. */
#define BUF_SIZE 256
#define PI acos(-1.0)
// need to define this macro as functions we're using are not C99
// compliant
#define _POSIX_C_SOURCE 200112L


/* Function declarations */

/* Function declarations for lander.c functions. */


/* Function declarations for utils.c functions. */
bool isNumber(char * );
bool setParameters(int , char ** );
void setupLand( FILE * , char *);
void setupShip();
void updateCenter();
void backupShipCoords();
bool readKey(int );
void eraseOldShip();
void transToZero();
void transRestore();
void openSketchpad();
 
/* Function declarations for elements.c functions. */
void makeLand();
void makeShip();
void rotateShip(double );
void thrustShip();
void gravityUpdate();

/* Function declarations for curses.c functions. */
int runCurses();
void setupCurses();
void unsetCurses();

/* Function declarations for timer.c functions. */
// our function which we will have called whenever the timer expires
void handleTimeout(int signal);
void blockSignal(int );
bool timer(int );

/* Structure for Figures */
struct figure
{
  //char name[BUF_SIZE];
  double oldCoords[BUF_SIZE]; 
  double newCoords[BUF_SIZE];
  double centerCoords[2];
  double velocity;
  double time;
  int numOldCoords;
  int numNewCoords;
};

/* Global Variables */
extern double gravity;
extern double thrust;
extern struct figure ship;
extern struct figure land;
extern double CENT_X;
extern sigset_t block_mask_g;
extern FILE * sketcher;
extern double timerIncrement;



/* Non-C99 compliant function prototypes. */
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
int sigemptyset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
// typedef struct siginfo_t {
//   int      si_signo;    /* Signal number */
//   int      si_errno;    /* An errno value */
//   int      si_code;     /* Signal code */
//   int      si_trapno;   /* Trap number that caused
// 			  hardware-generated signal
// 			  (unused on most architectures) */
//   pid_t    si_pid;      /* Sending process ID */
//   uid_t    si_uid;      /* Real user ID of sending process */
//   int      si_status;   /* Exit value or signal */
//   clock_t  si_utime;    /* User time consumed */
//   clock_t  si_stime;    /* System time consumed */
//   sigval_t si_value;    /* Signal value */
//   int      si_int;      /* POSIX.1b signal */
//   void    *si_ptr;      /* POSIX.1b signal */
//   int      si_overrun;  /* Timer overrun count; POSIX.1b timers */
//   int      si_timerid;  /* Timer ID; POSIX.1b timers */
//   void    *si_addr;     /* Memory location which caused fault */
//   long     si_band;     /* Band event (was int in
// 			  glibc 2.3.2 and earlier) */
//   int      si_fd;       /* File descriptor */
//   short    si_addr_lsb; /* Least significant bit of address
// 			  (since kernel 2.6.32) */
// }siginfo_t;
// struct sigaction {
//   void     (*sa_handler)(int);
//   void     (*sa_sigaction)(int, siginfo_t *, void *);
//   sigset_t sa_mask;
//   int      sa_flags;
//   void     (*sa_restorer)(void);
// };
int sigaction(int signum, const struct sigaction *act, 
  struct sigaction *oldact);




#endif