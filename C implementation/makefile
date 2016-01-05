
Solve : Solve.o Operation.o Errors.o
	gcc -lm -o Solve Solve.o Operation.o Errors.o
	clear

Solve.o: Solve.c
	gcc -c -g -std=c99 -Wall Solve.c

Operation.o : Operation.c Operation.h 
	gcc -c -g -std=c99 -Wall Operation.c
	
Errors.o: Errors.c Errors.h
	gcc -c -g -std=c99 -Wall Errors.c

clean :
	rm -f Solve Solve.o Operation.o Errors.o
 
    
