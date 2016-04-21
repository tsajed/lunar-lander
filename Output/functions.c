/*
name                    TANVIR SAJED
ONE Card number         1263382
Unix id                 tsajed
lecture section         A1
instructor's name       Martin Mueller
lab section             D05
TA's name               Van hai Ho

*/

//functions.h contains the function defintions that are required by
//the normal version of the game. The normal version does not use
//time functions such as signal.h functions because the normal
//version implements a loop to mimick the 0.05 second time elapse.

#include "struct.h"
#include "functions.h"
#include "functions_i.h"



int ListenKey(landerdata *data) {
    
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    InitLander(data);
    InitMap(data);

    fflush(data->p);
    
    printw("Press any key to start\n");
    printw("Then press arrow keys to rotate, "); 
    printw("space for thrust, 'q' to quit.\n");


    getch();
    nodelay(stdscr, TRUE);

    erase();
    printw("left arrow key rotates counter-clockwise, ");
    printw("right clockwise, space for thrust, q to quit.\n");

    
    while(1) {
        
        FindCentre(data);
        
        int w = 0;
        int key = ERR;
         while (w < 22500) {
            
            int ch = getch();
            
            if ((ch==KEY_LEFT)||(ch==KEY_RIGHT)||(ch== ' ')||(ch=='q'))
              key = ch;
            
            w++;
            }
        
        // printw("w is %d and key is %d\n", w, key);
        if (key == ERR) {
            
            UpdateLander(data);
        }
        
        else if(key == KEY_LEFT) {
            
            Call_LeftKey(data);
        }
        
        else if (key == KEY_RIGHT) {
            
            Call_RightKey(data);
        }
        
        else if (key == ' ') {
            
            Call_Thrust(data);
        }
        
        else if (key == 'q' ) {
            
          break;
        }
        
        else {
            
            UpdateLander(data);
        }
        
        int f = CheckLanding(data);
        
        if (f == 2 || f == 1) {

            End(data);
        }
        
    }
    
    endwin();
    
    return 1;
}



int UpdateLander(landerdata *data) {

  double x1, y1;
    
    for (int i = 1; i <= data->l_coord[0]; i = i+2) {
        
        fprintf(data->p, "eraseSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[i]), lround(data->l_coord[i+1]),
                lround(data->l_coord[i+2]), lround(data->l_coord[i+3]));
                       
       x1 = data->l_coord[i] + (data->xV * 0.05);
                            
       y1 = data->l_coord[i+1] + (data->yV * 0.05)
                            + (0.5 * data->gravity * 0.05 * 0.05);
        
       data->l_coord[i] = x1;
        data->l_coord[i+1] = y1;
                            
    }

    
    for (int i = 1; i <=(data->l_coord[17]-2); i = i + 2) {
        
        fprintf(data->p, "drawSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[i]), lround(data->l_coord[i+1]),
                lround(data->l_coord[i+2]), lround(data->l_coord[i+3]));
    }

    fflush(data->p);
    
    data->yV = data->yV + data->gravity * 0.05 ;
    
    return 1;
}


int Call_Thrust(landerdata *data) {

     double x1, y1;
    double xF = data->thrust * cos(data->angle);
    double yF = data->gravity + (data->thrust * sin(data->angle));
    
    for (int i = 1; i <= data->l_coord[0]; i = i + 2) {
        
        fprintf(data->p, "eraseSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[i]), lround(data->l_coord[i+1]),
                lround(data->l_coord[i+2]), lround(data->l_coord[i+3]));
                       
        x1 = data->l_coord[i] + (data->xV * 0.05)
                            + (0.5 * xF * 0.05 * 0.05);
                            
       y1 = data->l_coord[i+1] + (data->yV * 0.05)
                            + (0.5 * yF * 0.05 * 0.05);

       data->l_coord[i] = x1;
       data->l_coord[i+1] = y1;
    }
    
    
    for (int i = 1; i <=(data->l_coord[0]-2); i = i + 2) {
        
        fprintf(data->p, "drawSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[i]), lround(data->l_coord[i+1]),
                lround(data->l_coord[i+2]), lround(data->l_coord[i+3]));
    }

    fflush(data->p);

    data->fgc = data->fgc + 2;
    
    data->xV = data->xV + (xF * 0.05);               
    data->yV = data->yV + (yF * 0.05);
    
    return 1;
}


int Call_RightKey(landerdata *data) {
       
    data->angle = data->angle + (PI/18);
    
    for (int i = 1; i <= data->l_coord[0]; i = i + 2) {
        
        fprintf(data->p, "eraseSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[i]), lround(data->l_coord[i+1]),
                lround(data->l_coord[i+2]), lround(data->l_coord[i+3]));
                       
        rotate(data, i, (PI/18));
                                             
         data->l_coord[i] = data->l_coord[i] + (data->xV * 0.05);
                           
        data->l_coord[i+1] = data->l_coord[i+1] + (data->yV * 0.05)
        + (0.5 * data->gravity * 0.05 * 0.05);

    }


    
     for (int i = 1; i <=(data->l_coord[17]-2); i = i + 2) {
        
        fprintf(data->p, "drawSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[i]), lround(data->l_coord[i+1]),
                lround(data->l_coord[i+2]), lround(data->l_coord[i+3]));
        
     }

     fflush(data->p);
    data->yV = data->yV + data->gravity * 0.05 ;
    
    return 1;
}


int rotate(landerdata *data, int i, double angle_radians) {

  double x1,y1;
  
    data->l_coord[i] = data->l_coord[i] - data->centre_x;
    data->l_coord[i+1] = data->l_coord[i+1] - data->centre_y;
    
    x1  = (data->l_coord[i] *
                        cos(angle_radians)) -
                        (data->l_coord[i+1] *
                         sin(angle_radians));
    
            
    y1 = (data->l_coord[i] *
                          sin(angle_radians)) +
                         (data->l_coord[i+1] *
                          cos(angle_radians));

        
    data->l_coord[i] = x1 + data->centre_x;
    data->l_coord[i+1] = y1 + data->centre_y;
    
    
    return 1;
}


int Call_LeftKey(landerdata *data) {
    
    data->angle = data->angle -(PI/18);
    
    for (int i = 1; i <= data->l_coord[0]; i = i + 2) {
        
        fprintf(data->p, "eraseSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[i]), lround(data->l_coord[i+1]),
                lround(data->l_coord[i+2]), lround(data->l_coord[i+3]));
                       
        rotate(data, i, -(PI/18));
                                             
        data->l_coord[i] = data->l_coord[i] + (data->xV * 0.05);
                            
        data->l_coord[i+1] = data->l_coord[i+1] + (data->yV * 0.05)
                            + (0.5 * data->gravity * 0.05 * 0.05);

    }
    
    for (int i = 1; i <=(data->l_coord[17]-2); i = i + 2) {
        
        fprintf(data->p, "drawSegment %ld %ld %ld %ld\n",
                lround(data->l_coord[i]), lround(data->l_coord[i+1]),
                lround(data->l_coord[i+2]), lround(data->l_coord[i+3]));
    }
    
    fflush(data->p);
    
    data->yV = data->yV + data->gravity * 0.05 ;
    
    return 1;
}

int InitLander(landerdata *data) {
    
    data->l_coord[0] = 16;
    data->l_coord[1] = 315;
    data->l_coord[2] = 2;
    data->l_coord[3] = 310;
    data->l_coord[4] = 22;
    data->l_coord[5] = 330;
    data->l_coord[6] = 22;
    data->l_coord[7] = 325;
    data->l_coord[8] = 2;
    data->l_coord[9] = 315;
    data->l_coord[10] = 2;
    data->l_coord[11] = 310;
    data->l_coord[12] = 22;
    data->l_coord[13] = 320;
    data->l_coord[14] = 27;
    data->l_coord[15] = 330;
    data->l_coord[16] = 22;
    data->l_coord[17] = 10;
    
    for (int i = 1; i <= (data->l_coord[17] - 2); i = i + 2) {
        
        fprintf(data->p, "drawSegment %ld %ld %ld %ld\n",
               lround(data->l_coord[i]), lround(data->l_coord[i+1]),
               lround(data->l_coord[i+2]), lround(data->l_coord[i+3]));
    }
    
    data->angle = PI / 2;
    data->xV = 0;
    data->yV = 0;
    
    FindCentre(data);
    
    return 1;
}



int Intersection(landerdata *data, int i, int j) {

  if (data->l_coord[i+2] == data->l_coord[i])
    return 0;

  if (data->m_coord[j+2] == data->m_coord[j])
    return 0;
    
    double g1 = (data->l_coord[i+3] - data->l_coord[i+1]) /
                (data->l_coord[i+2] - data->l_coord[i]) ;
                
    double g2 = (data->m_coord[j+3] - data->m_coord[j+1]) /
                (data->m_coord[j+2] - data->m_coord[j]);
                
    double c1 = data->l_coord[i+3] - (g1 * data->l_coord[i+2]);
    
    double c2 = data->m_coord[j+3] - (g2 * data->m_coord[j+2]);
    
    if (g1 == g2) {
        
        double g3 = (data->l_coord[i+3] - data->m_coord[j+3]) /
                    (data->l_coord[i+2] - data->m_coord[j+2]);
                    
        if (g1 != g3)
            return 0;
        
        else {
            
            int k = Find_MinMax(data, i, j);
            
            if ((g2 == 0) && (k == 1))
                return k+1;
            
            else return k;
        }
    }
    
    else {
        
        double x = (c2 - c1)/(g1 - g2);
        
        int k = Find_IPoint(data, i, j, x);
        
        return k;
    }
}


int Find_MinMax(landerdata *data, int i, int j) {
    
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
    
    if (data->m_coord[j+1] > data->m_coord[j+3]) {
        
        min_my = data->m_coord[j+3];
        max_my = data->m_coord[j+1];
    }
    
    else {
        
        min_my = data->m_coord[j+1];
        max_my = data->m_coord[j+3];
    }
    
    if (data->m_coord[j] > data->m_coord[j+2]) {
        
        min_mx = data->m_coord[j+2];
        max_mx = data->m_coord[j];
    }
    
    else {
        
        min_mx = data->m_coord[j];
        max_mx = data->m_coord[j+2];
    }
    
    if ((min_lx > max_mx) ||( max_lx < min_mx)) {
        return 0;
    }
    
    else if ((min_ly > max_my) || (max_ly < min_my)) {
        return 0;
    }
    
    else return 1;

}


int Find_IPoint(landerdata *data, int i, int j, double x) {
    
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
    
    if (data->m_coord[j+1] > data->m_coord[j+3]) {
        
        min_my = data->m_coord[j+3];
        max_my = data->m_coord[j+1];
    }
    
    else {
        
        min_my = data->m_coord[j+1];
        max_my = data->m_coord[j+3];
    }
    
    if (data->m_coord[j] > data->m_coord[j+2]) {
        
        min_mx = data->m_coord[j+2];
        max_mx = data->m_coord[j];
    }
    
    else {
        
        min_mx = data->m_coord[j];
        max_mx = data->m_coord[j+2];
    }
    
    if (x <= max_mx && x <= max_lx && x >= min_lx && x >= min_mx) {
        return 1;
    }
    
    else return 0;
}


int CheckLanding(landerdata *data) {
    
    for (int i = 1; i <= (data->l_coord[17]-2); i = i + 2) {
        
        for( int j = 1; j <= (data->m_coord[0]-2); j = j+2) {
            
            int k = Intersection(data, i , j);
            
            if (k == 2) {
            
                    double vel = sqrt( (data->xV * data->xV) +
                                      (data->yV * data->yV));
                    
                    if ( vel < 20 && data->angle == PI/2 ) {

                      move(7,0);
                        printw("LANDED!!\n");
                        return 2;
                    }
                     else {
                       
                       move(7,0);
                        printw("CRASHED!!\n");
                        return 1;
                     }
            }  
            else if(k == 1) {

              move(7,0);
                printw("CRASHED on landscape\n");
                return 1;
            }
        }
    }

    return 0;
}

