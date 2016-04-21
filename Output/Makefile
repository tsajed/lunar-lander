
# name                    TANVIR SAJED
# ONE Card number         1263382
# Unix id                 tsajed
# lecture section         A1
# instructor's name       Martin Mueller
# lab section             D05
# TA's name               Van hai Ho


# Makefile that compiles game using make



lander : lander.o functions.o functions_i.o struct.h functions.h functions_i.h
	gcc -Wall -std=c99 -o lander lander.o functions.o functions_i.o -lcurses -lm

lander.o : lander.c struct.h functions.h functions_i.h
	gcc -Wall -std=c99 -c lander.c

functions.o : functions.c struct.h functions.h
	gcc -Wall -std=c99 -c functions.c
 
functions_i.o : functions_i.c struct.h functions_i.h functions.h
	gcc -Wall -std=c99 -c functions_i.c

clean :
	 -rm -f *.o lander core