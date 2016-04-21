#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>



int main(int argc, char *argv[]) {
    
    initscr();
    cbreak();				
	keypad(stdscr, TRUE);		
	noecho();
	nodelay(stdscr, TRUE);

    int x = 0;
    while(1) {
            int w = 0;    
        while(w < 22500) {
	    
	    int ch = getch();
	    w++;

	    if ((ch == KEY_UP )|| (ch == KEY_DOWN) || (ch == KEY_LEFT)) {
		break;
	    }
	}
    x++;
    if (x == 200) {
      printw("yes\n");
    }
        
    refresh();
    
    }
    getch();
    endwin();

  /*  int x = 0;			       
    while(1) {
	
    
    int w = 0;
    
	while(w < 26315) {
	
	    w++;	
	
	}
    x++;
    
    printf("x is %d\n", x);
    
    }*/
    
    return 0;
    
}

