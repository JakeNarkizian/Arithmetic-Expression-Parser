#ifndef OPERATION_H_
#define OPERATION_H_

#define MAX_LEN 512

typedef struct OperationListObj* OpList;
typedef struct OperationObj* Operation;


// Constructors-Destructors --------------------------------------------------- 
OpList newOpList(double eql, int has_var);

void freeOpList(OpList* pL);


// Access functions ----------------------------------------------------------- 
int opListLen(OpList L);

int opListIdx(OpList L);

Operation curOp(OpList L);

int prc(Operation O);

Operation opListEql(OpList L);


// Manipulation procedures ---------------------------------------------------- 
void clear(OpList L);

void setEql(OpList L, double val); 

Operation toOp(OpList L, int idx);

void prependOp(OpList L, char operator, ...);

void appendOp(OpList L, char operator, ...);

void addOpBefore(OpList L, char operator, ...);

void addOpAfter(OpList L, char operator, ...);

void deleteCurOp(OpList L);

void addPrc(OpList L,int amount);

void clearPrc(OpList L);

void setLeftOperand(OpList L, int operatorIdx);

void setRightOperand(OpList L, int operatorIdx);

void setOperands(OpList L, int operatorIdx);

int hasVar(Operation O);

double evalOp(Operation O,...);


// Other operations ----------------------------------------------------------- 

void printOpList(FILE* out, OpList L);

OpList argToOpList(int argc, char **argv);

void setAllOperands(OpList L);

double evalList(OpList L);

double solveVar(OpList L);

#endif
