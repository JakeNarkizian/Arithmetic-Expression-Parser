#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Operation.h"
#include "Errors.h"

int main(int argc, char **argv){
    if (argc < 2) usageError(argv[0],"v","\"[EXPRESSION]\"");
    if (!strcmp(argv[1],"-v") && argc != 4) usageError(argv[0],"v","\"[EXPRESSION]\" [EQUAL NUMBER]");
    
    OpList ops = argToOpList(argc, argv);
    if (!strcmp(argv[1],"-v")){
        setEql(ops, atof(argv[3]));   
        printf("x = %.2f\n",solveVar(ops));
    }else{
        if (hasVar(toOp(ops,0))) usageError(argv[0],"v","\"[EXPRESSION]\" [EQUAL NUMBER]");
        printf("= %.2f\n",evalList(ops));
    }
     freeOpList(&ops);
    

    return EXIT_SUCCESS;
}