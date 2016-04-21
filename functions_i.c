/*
name                    TANVIR SAJED
ONE Card number         1263382
Unix id                 tsajed
lecture section         A1
instructor's name       Martin Mueller
lab section             D05
TA's name               Van hai Ho

*/


//functions_i.c contains all the functions that are required by
//the improved version of the game.

#include "struct.h"
#include "functions_i.h"
#include "functions.h"


sigset_t block_mask_g;



landerdata *dat, *ast;

void TimeoutHandler(int);

int keymain;

double timed;

int InitTimer(landerdata* data) {


    // just initializes so it's empty (man 3 sigemptyset)
  sigemptyset(&block_mask_g); 
  // block_mask_g will now indicate to signal functions that something
  // (i.e., blocking) must be to SIGALRM signal
  sigaddset(&block_mask_g, SIGALRM); 

  // set up signal handler (man 2 sigaction)
  /* *********************************************************** */

  // will store information for our alarm
  struct sigaction handler;

  // handle_timeout is function to call when signal (in our case an
  // alarm) is sent
  handler.sa_handler = TimeoutHandler;
  // empties sa_mask, meaning no signals will be blocked when our
  // signal_handler (handle_timeout) is executing
  sigemptyset(&handler.sa_mask);
  // if non-zero, indicates various options ... we don't want to use
  // them
  handler.sa_flags = 0;

  // sets up a signal handler as specified in handler struct.
  // when SIGALRM occurs (set up below), handle_timeout will now be
  // called.
  if (sigaction(SIGALRM, &handler, NULL) == -1)
    exit(EXIT_FAILURE);

  // set up timer
  /* *********************************************************** */

  // sets up values for timer (intervals between sending SIGALRM).
  // times in microseconds (1/1 millionth of a second).
  // note although close enough for us, this is only approximate.
  // (man 2 setitimer)
  struct itimerval timer;

    // value for time between sending SIGALRM
    // The sample program set tv_usec to be 50000 (and tv_sec to be
    // zero), which meant the ship was redrawn approximately every
    // 50000 microseconds
    struct timeval time_delay;
    time_delay.tv_sec = 0;
    time_delay.tv_usec = 50000;

    timer.it_interval = time_delay;

    // value for initial time (before 1st alarm is triggered)
    struct timeval start;
    start.tv_sec = 0;
    start.tv_usec = 50000;

    timer.it_value = start;

  // will execute repeatedly, sending SIGALRM every timer.it_interval
  // microseconds, who initial value is timer.it_value.
  // should handle error better (and should use a macro).  should use
  // errno.  note it may save you some hassle if you do this, too

    keymain = ERR;  
  dat = data;
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();

    InitLander(data);
     InitMap(data);
   InitGauge(data);
   ast = InitAsteroids(data);
  
    
    printw("Press any key to start\n");
    printw("Then press arrow keys to rotate, "); 
    printw("space for thrust, 'q' to quit.\n");


    getch();
    nodelay(stdscr, TRUE);

    erase();
    printw("left arrow key rotates counter-clockwise, ");
    printw("right clockwise, space for thrust, q to quit.\n");

    move(3, 5);
    printw("Improved Version!!!\n");

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
       exit(EXIT_FAILURE);

    timed = 0;
 

  // infinite loop
  while(1) {
     
    int ch = getch();
  
     if ((ch==KEY_LEFT)||(ch==KEY_RIGHT)||(ch==' '))
       keymain = ch;

     else if (ch == 'q') 
       return 0;
  }
}



// called when SIGALRM is sent.
// could be called for other signals, so signal parameter indicates
// which one.
void TimeoutHandler(int signal)
{
  static int called = 0;

  called++;
  timed++;

  // called because of SIGALRM
  if (signal == SIGALRM)
  {
    // gets timer, puts it into timer (man 2 getitimer)
    struct itimerval timer;
    if (getitimer(ITIMER_REAL, &timer) == -1)
      exit(EXIT_FAILURE);

    // stops timer on 5th call.  In asn3, you should do this when the
    // game is done (e.g., ship has crashed or landed)

    int second = timed/20;
    Timer(second);
    
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
      exit(EXIT_FAILURE);

    SignalBlocker(dat, keymain);
  }

}



void SignalBlocker(landerdata *data, int key)
{
  // remember old list of signals to block (none, in our case)
  sigset_t old_mask;
  // set blocked signal set to block_mask_g (man 2 sigprocmask).
  // so now block SIGALRM.
  if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0)
    exit(EXIT_FAILURE);

  FindCentre(data);
  UpdateAsteroid(ast, data);
  
  if (key == ERR) {
       UpdateLander(data);
     
  }     
  else if(key == KEY_LEFT) 
       Call_LeftKey(data);
      
  else if (key == KEY_RIGHT) 
       Call_RightKey(data);
        
  else if (key == ' ') 
       Call_Thrust(data);
        
  else
       UpdateLander(data);
        
   int f = CheckLanding(data);
   keymain = ERR;

   if (data->fgc < 100)
     data->fgc++;

   else {
     data->fgc = 0;
     
     if(UpdateGauge(data)== 0) 
       End(data);
   }
        
  if (f == 2 || f == 1) {

        End(data);
  }

  AstLandCrash(data, ast);

  // unblock signal by setting mask to old value (which in our
  // case wasn't blocking anything)
  if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
    exit(EXIT_FAILURE);
}


void InitGauge(landerdata *data) {

  fprintf(data->p, "drawSegment 450 5 460 5\n");
  fprintf(data->p, "drawSegment 460 5 460 30\n");
  fprintf(data->p, "drawSegment 460 30 450 30\n");
  fprintf(data->p, "drawSegment 450 30 450 5\n");

  data->fg1 = 5;
  data->fg2 = 30;
  data->fgc = 0;

  for (int i = 5; i <= 30; i++) {

    fprintf(data->p, "drawSegment 450 %d 460 %d\n", i, i);
  }

}


int UpdateGauge(landerdata *data) {

  fprintf(data->p, "eraseSegment 450 %d 460 %d\n", data->fg1, data->fg1);

  if(data->fg1 == data->fg2) {

    move(7,0);
    printw("OUT OF FUEL\n");
    return 0;
  }

  data->fg1++;  
  return 1;
}


void End(landerdata *data) {

  while(1){

    nodelay(stdscr, FALSE);
    int end = getch();

    if (end == 'q') {
     fprintf(data->p, "end\n");
     pclose(data->p);
     endwin();
     exit(EXIT_SUCCESS);
    }
  }

}

landerdata *InitAsteroids(landerdata *data) {

  landerdata *ast = malloc(10 * (sizeof(landerdata)));

  for(int i = 0; i < 10; i++) {

    ast[i].gravity = data->gravity;
    
    ast[i].l_coord[0] = 8;
    ast[i].l_coord[1] = ((float)random()/RAND_MAX) * 600 ;
    ast[i].l_coord[2] = ((float)random()/RAND_MAX) * 600 - 1000;

    ast[i].l_coord[3] = ast[i].l_coord[1] + 10;
    ast[i].l_coord[4] = ast[i].l_coord[2] + 10;
    ast[i].l_coord[5] = ast[i].l_coord[1] + 20;
    ast[i].l_coord[6] = ast[i].l_coord[2] + 10;
    ast[i].l_coord[7] = ast[i].l_coord[1];
    ast[i].l_coord[8] = ast[i].l_coord[2];

    ast[i].xV = ((float)random()/RAND_MAX)*120 - 50;
    ast[i].yV = ((float)random()/RAND_MAX)*220;
  }

  return ast;

}

void UpdateAsteroid(landerdata *ast, landerdata *data) {

  double x1, y1;
  
  for(int j = 0; j < 10; j++) {
    
    for (int i = 1; i <= ast[j].l_coord[0]; i = i+2) {
        
        fprintf(data->p, "eraseSegment %ld %ld %ld %ld\n",
                lround(ast[j].l_coord[i]), lround(ast[j].l_coord[i+1]),
                lround(ast[j].l_coord[i+2]), lround(ast[j].l_coord[i+3]));
                       
       x1 = ast[j].l_coord[i] + (ast[j].xV * 0.05);
                            
       y1 = ast[j].l_coord[i+1] + (ast[j].yV * 0.05)
                            + (0.5 * data->gravity * 0.05 * 0.05);
        
       ast[j].l_coord[i] = x1;
        ast[j].l_coord[i+1] = y1;
                            
    }
  }

  for (int j = 0; j<10; j++) {
    for (int i = 1; i <=(ast[j].l_coord[0]-2); i = i + 2) {

       fprintf(data->p, "drawSegment %ld %ld %ld %ld\n",
                lround(ast[j].l_coord[i]), lround(ast[j].l_coord[i+1]),
               lround(ast[j].l_coord[i+2]), lround(ast[j].l_coord[i+3]));
        
    }
    
    ast[j].yV = ast[j].yV + ast[j].gravity * 0.05 ;
  }

    fflush(data->p);

}
    
int AstLandCrash(landerdata *data, landerdata *ast){

  for (int i = 1; i <=( data->l_coord[17]-2); i = i +2) {

    for( int j = 0; j < 10; j++) {

      for ( int k = 1; k <= (ast->l_coord[0]-2); k = k+2) {

        landerdata *aster = &ast[j];

        int l = InterAL(data, aster, i, k);
        if (l) {

          move(7,0);
          printw("Lander Destroyed by asteroid\n");
            End(data);
            }
          }
        } 
            
        }

    return 0;
}






int InterAL(landerdata *data,landerdata *ast,  int i, int j) {

    if (data->l_coord[i+2] == data->l_coord[i])
    return 0;

    if (data->m_coord[j+2] == data->m_coord[j])
    return 0;
    
    
    double g1 = (data->l_coord[i+3] - data->l_coord[i+1]) /
                (data->l_coord[i+2] - data->l_coord[i]) ;
                
    double g2 = (ast->l_coord[j+3] - ast->l_coord[j+1]) /
                (ast->l_coord[j+2] - ast->l_coord[j]);
                
    double c1 = data->l_coord[i+3] - (g1 * data->l_coord[i+2]);
    
    double c2 = ast->l_coord[j+3] - (g2 * ast->l_coord[j+2]);
    
    if (g1 == g2) {
        
        double g3 = (data->l_coord[i+3] - ast->l_coord[j+3]) /
                    (data->l_coord[i+2] - ast->l_coord[j+2]);
                    
        if (g1 != g3)
            return 0;
        
        else {
            
          int k = Find_ALMinMax(data,ast, i, j);
            
           return k;
        }
    }
    
    else {
        
        double x = (c2 - c1)/(g1 - g2);
        
        int k = Find_ALPoint(data,ast, i, j, x);
        
        return k;
    }
}


int Find_ALMinMax(landerdata *data,landerdata *ast, int i, int j) {
    
    double min_lx, max_lx, min_ly, max_ly;
    double min_mx, max_mx, min_my, max_my;
    
    if (data->l_coord[i] > data->l_coord[i+2]) {
        
        min_lx = data->l_coord[i+2];
        max_lx = data->l_coord[i];
    }
    
    else {
        
        min_lx = data->l_coord[i];
        max_lx = data->l_coord[i+2];
    }
    
    if (data->l_coord[i+1] > data->l_coord[i+3]) {
        
        min_ly = data->l_coord[i+3];
        max_ly = data->l_coord[i+1];
    }
    
    else {
        
        min_ly = data->l_coord[i+1];
        max_ly = data->l_coord[i+3];
    }
    
    if (ast->l_coord[j+1] > ast->l_coord[j+3]) {
        
        min_my = ast->l_coord[j+3];
        max_my = ast->l_coord[j+1];
    }
    
    else {
        
        min_my = ast->l_coord[j+1];
        max_my = ast->l_coord[j+3];
    }
    
    if (ast->l_coord[j] > ast->l_coord[j+2]) {
        
        min_mx = ast->l_coord[j+2];
        max_mx = ast->l_coord[j];
    }
    
    else {
        
        min_mx = ast->l_coord[j];
        max_mx = ast->l_coord[j+2];
    }
    
    if ((min_lx > max_mx) ||( max_lx < min_mx)) {
        return 0;
    }
    
    else if ((min_ly > max_my) || (max_ly < min_my)) {
        return 0;
    }
    
    else return 1;

}


int Find_ALPoint(landerdata *data, landerdata *ast, int i, int j, double x) {
    
    double min_lx, max_lx, min_ly, max_ly;
    double min_mx, max_mx, min_my, max_my;
    
    if (data->l_coord[i] > data->l_coord[i+2]) {
        
        min_lx = data->l_coord[i+2];
        max_lx = data->l_coord[i];
    }
    
    else {
        
        min_lx = data->l_coord[i];
        max_lx = data->l_coord[i+2];
    }
    
    if (data->l_coord[i+1] > data->l_coord[i+3]) {
        
        min_ly = data->l_coord[i+3];
        max_ly = data->l_coord[i+1];
    }
    
    else {
        
        min_ly = data->l_coord[i+1];
        max_ly = data->l_coord[i+3];
    }
    
    if (ast->l_coord[j+1] > ast->l_coord[j+3]) {
        
        min_my = ast->l_coord[j+3];
        max_my = ast->l_coord[j+1];
    }
    
    else {
        
        min_my = ast->l_coord[j+1];
        max_my = ast->l_coord[j+3];
    }
    
    if (ast->l_coord[j] > ast->l_coord[j+2]) {
        
        min_mx = ast->l_coord[j+2];
        max_mx = ast->l_coord[j];
    }
    
    else {
        
        min_mx = ast->l_coord[j];
        max_mx = ast->l_coord[j+2];
    }
    
    if (x <= max_mx && x <= max_lx && x >= min_lx && x >= min_mx) {
        return 1;
    }
    
    else return 0;
}


void Timer(int second) {

  move(5,5);
  int minute = 0;

  if (second >= 10 && second <= 59) {
    printw("Timer : 00:%d", second);

  }

  else if(second >= 60) {

    while(second >= 60) {

      second = second - 60;
      minute ++;

    }

    if (minute < 10) {

      if( second < 10) 
        printw("Timer : 0%d:0%d", minute, second);

      else
        printw("Timer : 0%d:%d", minute, second);
    }

    else {
      
      if( second < 10) 
        printw("Timer : %d:0%d", minute, second);

      else
        printw("Timer : %d:%d", minute, second);
    }
  }

  else
    printw("Timer : 00:0%d", second);
}
    



int TranslateX(landerdata *data, landerdata *ast) {

  for (int j = 0; j < 10; j++) {
    
    double max_a, min_a;
    max_a = Ast_FindMax(ast, j);

    if (max_a <= MIN_SKETCH) {

      

      for(int i = 1; i<= ast[j].l_coord[0]; i=i+2) {

         fprintf(data->p, "eraseSegment %ld %ld %ld %ld\n",
                lround(ast[j].l_coord[i]), lround(ast[j].l_coord[i+1]),
                lround(ast[j].l_coord[i+2]), lround(ast[j].l_coord[i+3]));

        ast[j].l_coord[i] = ast[j].l_coord[i] + MAX_SKETCH;

      }

      for (int i = 1; i <=(ast[j].l_coord[0]-2); i = i + 2) {

       fprintf(data->p, "drawSegment %ld %ld %ld %ld\n",
                lround(ast[j].l_coord[i]), lround(ast[j].l_coord[i+1]),
               lround(ast[j].l_coord[i+2]), lround(ast[j].l_coord[i+3]));

    }
    }

    min_a = Ast_FindMin(ast, j);

    if( min_a >= MAX_SKETCH) {


       for(int i = 1; j<= ast[j].l_coord[0]; i=i+2) {

          fprintf(data->p, "eraseSegment %ld %ld %ld %ld\n",
                lround(ast[j].l_coord[i]), lround(ast[j].l_coord[i+1]),
                lround(ast[j].l_coord[i+2]), lround(ast[j].l_coord[i+3]));

        ast[j].l_coord[i] = ast[j].l_coord[i] - MAX_SKETCH;

       }

       for (int i = 1; i <=(ast[j].l_coord[0]-2); i = i + 2) {

       fprintf(data->p, "drawSegment %ld %ld %ld %ld\n",
                lround(ast[j].l_coord[i]), lround(ast[j].l_coord[i+1]),
               lround(ast[j].l_coord[i+2]), lround(ast[j].l_coord[i+3]));
    }
  }
  }

  double max_l, min_l;

  min_l = FindCentre(data);

  max_l = (data->centre_x*2) - min_l;

  if (max_l <= MIN_SKETCH) {


      for(int j = 1; j<= data->l_coord[j]; j=j+2) {

        fprintf(data->p, "eraseSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[j]), lround(data->l_coord[j+1]),
                lround(data->l_coord[j+2]), lround(data->l_coord[j+3]));

        data->l_coord[j] = data->l_coord[j] + MAX_SKETCH;

      }

      for (int j = 1; j <=(data->l_coord[17]-2); j = j + 2) {
        
        fprintf(data->p, "drawSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[j]), lround(data->l_coord[j+1]),
                lround(data->l_coord[j+2]), lround(data->l_coord[j+3]));

    }
  }

    if( min_l >= MAX_SKETCH) {

       for(int j = 1; j<= data->l_coord[0]; j=j+2) {

          fprintf(data->p, "eraseSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[j]), lround(data->l_coord[j+1]),
                lround(data->l_coord[j+2]), lround(data->l_coord[j+3]));

         data->l_coord[j] = data->l_coord[j] - MAX_SKETCH;

       }

       for (int j = 1; j <=(data->l_coord[17]-2); j = j + 2) {
        
        fprintf(data->p, "drawSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[j]), lround(data->l_coord[j+1]),
                lround(data->l_coord[j+2]), lround(data->l_coord[j+3]));
    }
 }
}
  

double Ast_FindMin(landerdata *ast, int j) {

  double min = ast[j].l_coord[1];
   for (int i = 1; i <= ast[j].l_coord[0]; i = i + 2) {
	
	if (min > ast[j].l_coord[i]) {
            min = ast[j].l_coord[i];
	}
	
   }
   return min;
}
   
double Ast_FindMax(landerdata *ast, int j) {

  double max = ast[j].l_coord[2];
   for (int i = 1; i <= ast[j].l_coord[0]; i = i + 2) {
	
	if (max < ast[j].l_coord[i]) {
            max = ast[j].l_coord[i];
	}
	
   }
   return max;
}
    
