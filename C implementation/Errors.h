#ifndef ERRORS_H_
#define ERRORS_H_

void usageError(const char modName[],char* options,char* message);
void nullError(const char modName[], const char functionName[]);
void preconError(const char modName[],const char functionName[], char* message);

#endif