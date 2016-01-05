#include <stdio.h>
#include <stdlib.h>

#include "Errors.h"

void usageError(const char modName[],char* options,char* message){
    fprintf(stderr,"usage: %s [-%s] %s\n",modName,options,message);
    exit(1);
}

void nullError(const char modName[], const char functionName[]){
   printf("%s ERROR in %s():\n   calling %s on NULL reference.\n"
         ,modName,functionName,functionName);
   exit(1);
}

void preconError(const char modName[], const char functionName[], char* message){
   printf("%s ERROR in %s():\n    %s.\n"
          ,modName ,functionName, message);
   exit(1);
}
