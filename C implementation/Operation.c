#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "Operation.h"
#include "Errors.h"

#define NUM_ERR "invalid format: consecutive numbers"
#define OPR_ERR "invalid format: consecutive operators or operator without number"
#define DEC_ERR "invalid format: number with two decimal points"
#define OPN_ERR "invalid format: openning '(' without close"
#define CLS_ERR "invalid format: closing ')' without open"
#define CHR_ERR "invalid format: invalid char used"
#define ARG_ERR "invalid: not enough operations in function parameter"

#define UDEF -1
#define MAX_VAR_VAL 9999999999999978.99999
#define MIN_VAR_VAL -9999999999999978.99999

typedef struct OperationObj{
   char o; // operator
   struct OperationObj* l; // left operand
   struct OperationObj* r; // right operand
   struct OperationObj* nxt;
   struct OperationObj* prv;
   double n;
   int prc;  
}OperationObj;


typedef struct OperationListObj{
   Operation frt;
   Operation bck;
   Operation cur;
   int idx; // -1 is undefined 
   int len;
   Operation eql; 
   int has_var;
}OperationListObj;

// Constructors-Destructors --------------------------------------------------- 


/* 
 * PRIVATE FUNCTION: newOp(char operator, ...)
 *      Allocates memory for new Operation. If operator is 'i' (identification)
 *      an optional parameter for the number to be identified must be passed.
 *   
 * RETURNS: pointer to new Operation  
 * 
 * PRECONDITIONS:
 *      (1) operator must be 'i','-','^','*','/', or '+' 
 */ 
Operation newOp(char operator, ...){
    double n = 0;
    va_list operands;
    Operation new = malloc(sizeof(OperationObj));
    switch(operator){
        case 'i':
            va_start(operands,operator);
            n = va_arg(operands,double);
            new->prc = 6;
            break;
        
        case '^':
            new->prc = 5;
            break;
            
        case '*':
            new->prc = 4;
            break;
            
        case '/':
            new->prc = 3;
            break;
        case '+':
            new->prc = 2;
            break;
        case '-':
            new->prc = 1;
            break;    
        case 'x':
            new->prc = 0;
            break;    
        default:
            preconError(__FILE__,__func__,"invalid char");
            printf("Error\n");
            return NULL; 
            break;
    }
    va_end(operands);
    
    
    new->n = n;
    new->o = operator;
    new->l = NULL;
    new->r = NULL;
    new->nxt = NULL;
    new->prv = NULL;
    
    return new;
}



/* 
 * FUNCTION: newOpList()
 *      Allocates memory for new Operation List. 
 *   
 * RETURNS: pointer to new list  
 */ 
OpList newOpList(double eql, int has_var){
    OpList new = malloc(sizeof(OperationListObj));
    new->frt = NULL;
    new->bck = NULL;
    new->len = 0;
    new->idx = UDEF;
    new->cur = NULL;
    new->eql = newOp('i',eql);
    new->has_var = has_var;
    return new;
}

/* 
 * FUNCTION: freeOpList(OpList* pL)
 *      Frees memory pointed to by pL and all memory pointed to by elements 
 *      in the list.  
 *   
 *      NOTE: when using this function use & operator   
 *
 * RETURNS: void 
 */ 
void freeOpList(OpList* pL){
    if (!pL) nullError(__FILE__,__func__);
    
    clear(*pL);
    free (*pL); 
    *pL = NULL;
}

/* 
 * FUNCTION: freeOp(Operation* pOp)
 *      frees memory pointed to by pOp and all memory pointed to by elements 
 *      in the list.  
 *   
 * RETURNS: void 
 */ 
void freeOp(Operation* pOp){
    if (!pOp) nullError(__FILE__,__func__);
    free((*pOp)->l);
    free((*pOp)->r);
    free(*pOp);
    *pOp = NULL;
}

// Access functions ----------------------------------------------------------- 

/* 
 * FUNCTION: opListLen(OpList L)
 *
 * RETURNS: An int of the number of elements in L.
 */ 
int opListLen(OpList L){
    if (!L) nullError(__FILE__,__func__);
    
    return L->len;
}

/* 
 * FUNCTION: opListIdx(OpList L)
 *
 * RETURNS: An int of the index of the cursor of L.
 */ 
int opListIdx(OpList L){
    if (!L) nullError(__FILE__,__func__);
    
    return L->idx;
}


/* 
 * FUNCTION: curOp(OpList L)
 *
 * RETURNS: Operation pointed to by L's cursor.
 *
 * PRECONDITIONS:
 *      (1) length of L must be at least 1
 *      (2) cursor must not be undefined
 */ 
Operation curOp(OpList L){
    if (!L) nullError(__FILE__,__func__);
    if (L->len < 1) preconError(__FILE__,__func__,"Length of list must be greater than 1");
    if (L->idx == UDEF) preconError(__FILE__,__func__,"Cursor cannot be undefined");
    
    return L->cur;
}

/* 
 * FUNCTION: prc(Operation O) 
 *
 * RETURNS: O's precedence
 */
int prc(Operation O){
    if (!O) nullError(__FILE__,__func__);
    
    return O->prc;
}

/* 
 * FUNCTION: opListEql(OpList L) 
 *
 * RETURNS: L's eql value
 */
Operation opListEql(OpList L){
    if (!L) nullError(__FILE__,__func__);
    
    return L->eql;
}

// Manipulation procedures ---------------------------------------------------- 

/* 
 * HELPER FUNCTION: undefOpListCur(OpList L)
 *      Undefines L's cursor.
 *
 * RETURNS: void 
 */ 
void undefOpListCur(OpList L){
    if (!L) nullError(__FILE__,__func__);

    L->cur = NULL;
    L->idx = UDEF;
}

/* 
 * FUNCTION: clear(OpList L)
 *      Removes all Operations in L and resets it to default.
 *
 * RETURNS: void 
 */ 
void clear(OpList L){
    if (!L) nullError(__FILE__,__func__);
   
    Operation curr = L->frt;
    while (curr != NULL){
        Operation temp = curr;
        curr = curr->nxt;
        freeOp(&temp);
    }
    L->frt = NULL;
    L->bck = NULL;
    L->len = 0;
    L->eql = NULL;
    undefOpListCur(L);
}

/* 
 * FUNCTION: setEql(OpList L, double val)
 *      Sets eql of L to an id with value val.
 *
 * RETURNS: void 
 */ 
void setEql(OpList L, double val){
    if (!L) nullError(__FILE__,__func__);
    if ((L->eql) != NULL) freeOp(&(L->eql)); 
    L->eql = newOp('i',val);
}

/* 
 * FUNCTION: toOp(OpList L, int idx)
 *      Moves L's cursor to index idx. If idx is out of range of the 
 *      index's of L, the cursor becomes undefined. 
 *
 * RETURNS: Operation currently pointed to by L's cursor.  
 */ 
Operation toOp(OpList L, int idx){
    if (!L) nullError(__FILE__,__func__);
   
    if (idx < 0 || idx >= (L->len)){
        undefOpListCur(L);
    }else{
        L->cur = L->frt;
        for (int i = 0; i < idx; i++){
            L->cur = L->cur->nxt;
        }
        L->idx = idx;
    }
    return L->cur;
}

/* 
 * FUNCTION: toFront(OpList L)
 *      Moves L's cursor to the front of the list. 
 *
 * RETURNS: Operation currently pointed to by L's cursor.
 *
 * PRECONDITIONS:
 *      (1) length of L must be at least 1  
 */ 
Operation toFront(OpList L){
    if (!L) nullError(__FILE__,__func__);
    if (L->len <= 0) preconError(__FILE__,__func__,"Length of list must be greater than 1");
    
    L->cur = L->frt;
    L->idx = 0;
    return L->cur;
}



/* 
 * FUNCTION: toBack(OpList L)
 *      Moves L's cursor to the back of the list. 
 *
 * RETURNS: Operation currently pointed to by L's cursor.
 *
 * PRECONDITIONS:
 *      (1) length of L must be at least 1 
 */ 
Operation toBack(OpList L){
    if (!L) nullError(__FILE__,__func__);
    if (L->len <= 0) preconError(__FILE__,__func__,"Length of list must be greater than 1");
    
    L->cur = L->bck;
    L->idx = L->len-1;
    return L->cur;
}


/* 
 * FUNCTION: prvOp(OpList L)
 *      Decrements L's by one. If the cursor is undefined or points to the
 *      first element, it becomes undefined.  
 *
 * RETURNS: Operation currently pointed to by L's cursor. 
 */ 
Operation prvOp(OpList L){
    if (!L) nullError(__FILE__,__func__);

    if (L->idx <= 0){
        undefOpListCur(L);
   }else{
        toOp(L,L->idx - 1);
   }
   return L->cur;
}

/* 
 * FUNCTION: nxtOp(OpList L)
 *      Moves L's cursor foreward by one. If the cursor is undefined or points
 *      to the last element, it stays or becomes undefined.  
 *
 * RETURNS: void 
 */ 
Operation nxtOp(OpList L){
    if (!L) nullError(__FILE__,__func__);
    
    if (L->idx < 0 || L->idx == (L->len - 1)){
        undefOpListCur(L);
    }else{
        toOp(L,L->idx + 1);
    }
    return L->cur;
}

/* 
 * FUNCTION: prependOp(OpList L, char operator, ...)
 *      Creates new operation and prepends it to the front of L.     
 *
 * RETURNS: Operation currently pointed to by L's cursor.  
 */ 
void prependOp(OpList L, char operator, ...){
    if (!L) nullError(__FILE__,__func__);
    
    Operation toAdd;
    if (operator == 'i'){
        va_list operands;
        va_start(operands,operator);
        toAdd = newOp(operator,va_arg(operands,double));
        va_end(operands); 
    }else{
        toAdd = newOp(operator);
    }  
    
    if (L->len != 0){
        toAdd->nxt = L->frt;
        L->frt->prv = toAdd;
    }else{
        L->bck = toAdd;
    }
    L->frt = toAdd;
    L->len++;
    if (L->idx != UDEF) L->idx++;
}


/* 
 * FUNCTION: appendOp(OpList L, char operator, ...)
 *      Creates new operation and appends it to the back of L.     
 *
 * RETURNS: void 
 */ 
void appendOp(OpList L, char operator, ...){
    if (!L) nullError(__FILE__,__func__);
    
    Operation toAdd;
    if (operator == 'i'){
        va_list operands;
        va_start(operands,operator);
        toAdd = newOp(operator,va_arg(operands,double));
        va_end(operands); 

    }else{
        toAdd = newOp(operator);
    }  
    
    if (L->len != 0) L->bck->nxt = toAdd;
    else L->frt = toAdd;
    
    toAdd->prv = L->bck;
    L->bck = toAdd;
    L->len++;

}

/* 
 * FUNCTION: addOpBefore(OpList L, char Operator, ...)
 *      Creates new Operation and adds it to L before the cursor.    
 *
 * RETURNS: void 
 *
 * PRECONDITIONS:
 *      (1) length of L must be at least 1
 *      (2) cursor must not be undefined
 */ 
void addOpBefore(OpList L, char operator, ...){
    if (!L) nullError(__FILE__,__func__);
    if (L->len <= 0) preconError(__FILE__,__func__,"Length of list must be greater than 1");
    if (L->idx == UDEF) preconError(__FILE__,__func__,"Cursor cannot be undefined");
    
    Operation toAdd;
    if (operator == 'i'){
        va_list operands;
        va_start(operands,operator);
        toAdd = newOp(operator,va_arg(operands,double));
        va_end(operands); 
    }else{
        toAdd = newOp(operator);
    } 
    
    toAdd->nxt = L->cur;
    toAdd->prv = L->cur->prv;
    if (L->cur == L->frt) L->frt = toAdd;
    else L->cur->prv->nxt = toAdd;
    L->cur->prv = toAdd; 

    L->len++;
    L->idx++;
}

/* 
 * FUNCTION: addOpAfter(OpList L, char Operator, ...)
 *      Creates new Operation and adds it to L after the cursor.    
 *
 * RETURNS: void  
 *
 * PRECONDITIONS:
 *      (1) length of L must be at least 1
 *      (2) cursor must not be undefined
 */ 
void addOpAfter(OpList L, char operator, ...){
    if (!L) nullError(__FILE__,__func__);
    if (L->len <= 0) preconError(__FILE__,__func__,"Length of list must be greater than 1");
    if (L->idx == UDEF) preconError(__FILE__,__func__,"Cursor cannot be undefined");

    Operation toAdd;
    if (operator == 'i'){
        va_list operands;
        va_start(operands,operator);
        toAdd = newOp(operator,va_arg(operands,double));
        va_end(operands); 
    }else{
        toAdd = newOp(operator);
    } 
    
    toAdd->prv = L->cur;
    toAdd->nxt = L->cur->nxt;
    if (L->cur == L->bck) L->bck = toAdd;
    else L->cur->nxt->prv = toAdd;
    L->cur->nxt = toAdd; 

    L->len++;
}


/* 
 * FUNCTION: deleteCurOp(OpList L)
 *      Removes the Operation pointed to by L's cursor. The cursor is then 
 *      undefined. 
 *
 * RETURNS: void
 *
 * PRECONDITIONS:
 *      (1) length of L must be at least 1
 *      (2) cursor must not be undefined
 */  
void deleteCurOp(OpList L){
    if (!L) nullError(__FILE__,__func__);
    if (L->len <= 0) preconError(__FILE__,__func__,"Length of list must be greater than 1");
    if (L->idx == UDEF) preconError(__FILE__,__func__,"Cursor cannot be undefined");
    
    if (L->cur->nxt == NULL) L->bck = L->cur->prv;
    else L->cur->nxt->prv = L->cur->prv;
        
    if (L->cur->prv == NULL) L->frt = L->cur->nxt;
    else L->cur->prv->nxt = L->cur->nxt;
    
    L->len--;
    undefOpListCur(L);

}

/* 
 * FUNCTION: addPrc(OpList L,int amount)
 *      Increments the precedence of L's cursor by amount. 
 *
 * RETURNS: void
 * 
 * PRECONDITIONS:
 *      (1) length of L must be at least 1
 *      (2) cursor must not be undefined
 */  
void addPrc(OpList L,int amount){
    if (!L) nullError(__FILE__,__func__);
    if (L->len <= 0) preconError(__FILE__,__func__,"Length of list must be greater than 1");
    if (L->idx == UDEF) preconError(__FILE__,__func__,"Cursor cannot be undefined");
    
    L->cur->prc+=amount;
}

/* 
 * FUNCTION: clearPrc(OpList L)
 *      Sets the precedence of L's cursor to zero. 
 *
 * RETURNS: void
 * 
 * PRECONDITIONS:
 *      (1) length of L must be at least 1
 *      (2) cursor must not be undefined
 */  
void clearPrc(OpList L){
    if (!L) nullError(__FILE__,__func__);
    if (L->len <= 0) preconError(__FILE__,__func__,"Length of list must be greater than 1");
    if (L->idx == UDEF) preconError(__FILE__,__func__,"Cursor cannot be undefined");
    
    L->cur->prc=0;
}

/* 
 * FUNCTION: setLeftOperand(OpList L, int operatorIdx)
 *      Removes the operation left of the Operation at operatorIdx in L,
 *      and adds it as the left operand of the operation at operatorIdx.
 *       
 * RETURNS: void 
 *
 * PRECONDITIONS:
 *      (1) Operator cannot be id
 *      (2) Length must be at least 2
 *      (3) operatorIdx must be a valid index, but cannot be the first index
 */ 
void setLeftOperand(OpList L, int operatorIdx){
    if (!L) nullError(__FILE__,__func__);
    toOp(L,operatorIdx);
    if (curOp(L)->o == 'i') preconError(__FILE__,__func__,"Operator cannot be id");
    if (L->len < 2) preconError(__FILE__,__func__,"Length of L must be at least 2");
    if (operatorIdx <= 0 ) preconError(__FILE__,__func__,"Operator index is out of range");
    if (operatorIdx >= (L->len)) preconError(__FILE__,__func__,"Operator index is out of range");
    
    Operation operator = curOp(L);
    prvOp(L); 
    operator->l = curOp(L);
    deleteCurOp(L);
}


/* 
 * FUNCTION: setRightOperand(OpList L, int operatorIdx)
 *      Removes the operation right of the Operation at operatorIdx in L,
 *      and adds it as the right operand of the operation at operatorIdx.
 *       
 *
 * RETURNS: void 
 *
 * PRECONDITIONS:
 *      (1) Operator cannot be id
 *      (2) Length must be at least 2
 *      (3) operatorIdx must be a valid index, but cannot be the last index
 */ 
void setRightOperand(OpList L, int operatorIdx){
    if (!L) nullError(__FILE__,__func__);
    toOp(L,operatorIdx);
    if (curOp(L)->o == 'i') preconError(__FILE__,__func__,"Operator cannot be id");
    if (L->len < 2) preconError(__FILE__,__func__,"Length of L must be at least 2");
    if (operatorIdx < 0 ) preconError(__FILE__,__func__,"Operator index is out of range");
    if (operatorIdx >= (L->len)-1) preconError(__FILE__,__func__,"Operator index is out of range");

    Operation operator = curOp(L);
    nxtOp(L);
    operator->r = curOp(L);
    deleteCurOp(L);
}

/* 
 * FUNCTION: setLeftOperandTo(Operation O, Operation oper)
 *      Sets oper as left operand of O.
 *       
 * RETURNS: void 
 */
void setLeftOperandTo(Operation O, Operation oper){
    if (!O) nullError(__FILE__,__func__);
    if (!oper) nullError(__FILE__,__func__);
    
    O->l = oper;
}

/* 
 * FUNCTION: setRightOperandTo(Operation O, Operation oper)
 *      Sets oper as right operand of O.
 *       
 * RETURNS: void 
 */
void setRightOperandTo(Operation O, Operation oper){
    if (!O) nullError(__FILE__,__func__);
    if (!oper) nullError(__FILE__,__func__);
    
    O->r = oper;
}

/* 
 * FUNCTION: setOperands(OpList L, int operatorIdx)
 *      Removes the operations left and right of the Operation at operatorIdx 
 *      in L. They are then added as the operands for the operation at 
 *      operatorIdx.
 *
 * RETURNS: void 
 *
 * PRECONDITIONS:
 *      (1) Operator cannot be id
 *      (2) Operator cannot be id
 *      (3) Length must be at least 2
 *      (4) operatorIdx must be a valid index, but cannot be the last or first 
 *          index
 */ 
void setOperands(OpList L, int operatorIdx){
    if (!L) nullError(__FILE__,__func__);
    toOp(L,operatorIdx);
    if (curOp(L)->o == 'i') preconError(__FILE__,__func__,"Operator cannot be id");
    if (curOp(L)->o == 'x') preconError(__FILE__,__func__,"Operator cannot be a var");
    if (L->len < 2) preconError(__FILE__,__func__,"Length of L must be at least 2");
    if (operatorIdx <= 0 ) preconError(__FILE__,__func__,"Operator index is out of range");
    if (operatorIdx >= (L->len)-1) preconError(__FILE__,__func__,"Operator index is out of range");

    setLeftOperand(L,operatorIdx--);
    setRightOperand(L,operatorIdx);
}

/* 
 * FUNCTION: hasVar(Operation O)
 *      Returns a positive int (true in c) if O or any of O's child 
 *      Operations contain a variable. Otherwise 0 (false) is returned
 *       
 * RETURNS: 0 or a positive int 
 */
int hasVar(Operation O){
    if (!O) nullError(__FILE__,__func__);

    if (O->o == 'x') return 1;
    if (O->o == 'i') return 0;
    
    return hasVar(O->r) + hasVar(O->l);
}


/* 
 * FUNCTION: evalOp(Operation O)
 *      Takes an operation and evaluates it to a double. The function is 
 *      recursive, and each or a 'x' child operation of the original O will call 
 *      evalOp until an 'i' operation is reached. 'i' (short for id)
 *      evaluates to a double.  
 *
 *      NOTE: preconditions are checked for inside the body of the function
 *
 * RETURNS: The result of Operation as a double. 
 *
 * PRECONDITIONS:
 *      (1) Operation must have valid operator
 *      (2) if dividing, right operand cannot be zero
 *      (3) ^ cannot be used with an unknown
 */
double evalOp(Operation O,...){
    if (!O) nullError(__FILE__,__func__);
    
    double toRet = O->n;
    Operation temp;
    double denom;
    va_list tryVal;
    va_start(tryVal,O);
    double varVal = va_arg(tryVal,double);
    va_end(tryVal);

    switch(O->o){
        case 'i':
            
            return toRet;
            break;
            
        case 'x':

            return varVal;
            break;  
              
        case '-':
            temp = newOp('i', evalOp(O->l,varVal) - evalOp(O->r,varVal));
            toRet = evalOp(temp,varVal);
            freeOp(&temp);
            return toRet;
            break;
        
        case '^':
            if (hasVar(O)) preconError(__FILE__,__func__,"^ cannot be used with an unknown");
        
            temp = newOp('i', pow(evalOp(O->l,varVal),evalOp(O->r,varVal)));
            toRet = evalOp(temp,varVal);
            freeOp(&temp);
            return toRet;
            break;
            
        case '*':
            temp = newOp('i', evalOp(O->l,varVal) * evalOp(O->r,varVal));
            toRet = evalOp(temp,varVal);
            freeOp(&temp);
            return toRet;
            break;
            
        case '/':
            denom = evalOp(O->r,varVal);
            if (!denom) preconError(__FILE__,__func__,"Right operand cannot be zero in a division operation");
            if (hasVar(O->r)) preconError(__FILE__,__func__,"There cannot be an unknown value in the right operand of a division operation");

            temp = newOp('i', evalOp(O->l,varVal) / denom);
            toRet = evalOp(temp,varVal);
            freeOp(&temp);
            return toRet;
            break;
        
        case '+':    
            temp = newOp('i', evalOp(O->l,varVal) + evalOp(O->r,varVal));
            toRet = evalOp(temp,varVal);
            freeOp(&temp);
            return toRet;
            break;
               
        default:
            preconError(__FILE__,__func__,"Called with invalid char");
            return 0;
            break;
    }
} 








// Other operations ----------------------------------------------------------- 


/* 
 * HELPER FUNCTION: printOp(FILE* out, Operation O)
 *      Prints option to given output, used by printOpList function.   
 *
 * RETURNS: void 
 */ 
void printOp(FILE* out, Operation O){
    if (!out) nullError(__FILE__,__func__);
    if (!O) nullError(__FILE__,__func__);
    
    if (O->o == 'x') fprintf(out,"(X)");
    else
    if (O->o != 'i'){
        fprintf(out," (");
        
        if ((O->l) != NULL) printOp(out,(O->l));
        else fprintf(out,"_");
        
        fprintf(out," %c ",O->o);
        
        if ((O->r) != NULL) printOp(out,(O->r));
        else fprintf(out,"_");
        
        fprintf(out,") ");
    }else{
        fprintf(out,"( %.2f )",O->n);
    }
}

/* 
 * FUNCTION: printOpList(FILE* out, OpList L)
 *      Prints list to given output.   
 *
 * RETURNS: void 
 */ 
void printOpList(FILE* out, OpList L){
    if (!out) nullError(__FILE__,__func__);
    if (!L) nullError(__FILE__,__func__);
    
    fprintf(out,"[ ");
    for (Operation curr = L->frt; curr != NULL; curr = curr->nxt){
        printOp(stdout, curr);
        if (curr->nxt != NULL) fprintf(out,",");

    }
    fprintf(out,"] \n");
        
}

int num_strt = 0;
int dec_pnt  = 0;
int xtra_prc = 0;
int var = 0;
int lst_var = 0;
/* 
 * HELPER FUNCTION: void appendNum(OpList L, char nxt_num[])
 *      Appends an id operation to L. The number represented in nxt_num
 *      is used as the double. Precedence is also adjusted in the case
 *      of parentheses. This function is used by argToOpList.   
 *
 * RETURNS: void 
 */ 
void appendNum(OpList L, char nxt_num[]){
    appendOp(L, 'i', atof(nxt_num));
    
    memset(nxt_num, 0, MAX_LEN);
    toBack(L);
    addPrc(L, xtra_prc);
    lst_var = 0;
}

/* 
 * HELPER FUNCTION: appendOperator(OpList L, char op)
 *      Appends operation op to L. Precedence is also adjusted in the case
 *      of parentheses. This function is used by argToOpList. 
 *
 * RETURNS: void 
 */ 
void appendOperator(OpList L, char op){
    appendOp(L,op);
    toBack(L);
    addPrc(L,xtra_prc);
    num_strt = 0;
    dec_pnt = 0;
    lst_var = 0;
}

/* 
 * HELPER FUNCTION: startNum(int j)
 *      Adjusts global variables and returns given int. Mostly syntactic 
 *      sugar. This function is used by argToOpList. 
 *
 * RETURNS: The passed int j 
 */ 
int startNum(int j){
    num_strt = 1;
    dec_pnt = 0;
    return j;
}

/* 
 * FUNCTION: argToOpList(int argc, char **argv)
 *      Takes an array of strings and its length and parses char 
 *      representations of operations into an OpList.  
 *
 *      NOTES: 
 *             (1) This function checks for preconditions mostly in its body 
 *                 rather than at the very beginning as this is more efficient 
 *                 and more readable. 
 *
 *             (2) The '-' is treated as a subtraction operation if a number
 *                 was last parsed. Otherwise it is a negative sign and parsed 
 *                 as part of a number  
 *
 *             (3) Parens alter precedence by 5, ensuring that the inner 
 *                 operations precede outer ones regardless of default 
 *                 operator precedence
 *
 * RETURNS: OpList containing all operations represented in argv
 *
 * PRECONDITIONS:
 *      (1) argv must contain at least one operation
 *      (2) no invalid chars
 *      (3) no consecutive numbers
 *      (4) no consecutive operators
 *      (5) no consecutive negative signs for a single number
 *      (6) no consecutive decimal points for a single number
 *      (7) each openning paren must be matched with a closing paren
 *      (8) only one unknown is allowed
 */
OpList argToOpList(int argc, char **argv){
    if (argc < 2) preconError(__FILE__,__func__,ARG_ERR); 
    
    OpList ops = newOpList(0.0,0);
    char nxt_num[MAX_LEN];
    num_strt = 0;
    dec_pnt  = 0;
    xtra_prc = 0;
    int idx;
    int i = 1;
    if(!strcmp(argv[1],"-v")) i = 2;
    for (int j = 0; argv[i][j] != '\0'; j++){
        switch(argv[i][j]){           
            // --> NUMBERS 
            case '0': case '1': case '2': case '3': case '4': 
            case '5': case '6': case '7': case '8': case '9': 
                if (num_strt == 2) preconError(__FILE__,__func__,NUM_ERR); 
                                  
                if (!num_strt) idx = startNum(j);
                nxt_num[j - idx] = argv[i][j];
                break;
                   
            case '-':              
                if (num_strt){
                    if (!strcmp(nxt_num, "-")) preconError(__FILE__,__func__,OPR_ERR);
                        
                    if (!lst_var) appendNum(ops,nxt_num);    
                    appendOperator(ops,'-');
                }else{
                    if (!num_strt) idx = startNum(j);
                    nxt_num[j - idx] = argv[i][j];
                }
                break;
                                 
            case '.': 
                if (num_strt == 2) preconError(__FILE__,__func__,NUM_ERR); 
                if (!num_strt) idx = startNum(j);
                             
                if (dec_pnt) preconError(__FILE__,__func__, DEC_ERR);
                dec_pnt = 1;
                nxt_num[j - idx] = argv[i][j];
                break;    
                                        
            // --> UNKNOWN   
            case 'x':
                if (var) preconError(__FILE__,__func__,"Only one unknown is allowed");
                if (num_strt && nxt_num[j-idx -1] == '-' ) preconError(__FILE__,__func__,OPR_ERR);
                
                 
                if (num_strt){
                    appendNum(ops,nxt_num);
                    appendOperator(ops, '*');
                }
                appendOperator(ops,'x'); 
                var = 1; 
                ops->has_var = 1;
                lst_var = 1;
                num_strt = 2;
                break;
            
            // --> OPERATIONS
            case '^': case '*': case '/': case '+': 
                if (!num_strt) preconError(__FILE__,__func__,OPR_ERR);

                if (!lst_var)appendNum(ops,nxt_num);
                appendOperator(ops, argv[i][j]);
                break;
                
            // --> PARENS     
            case '(':
                if (num_strt == 1){
                    appendNum(ops,nxt_num);
                    appendOperator(ops,'*');
                }
                xtra_prc += 5;
                continue;
                
            case ')':
                if (xtra_prc < 1) preconError(__FILE__,__func__,CLS_ERR);
                      
                xtra_prc -= 5;
                if (num_strt == 1){
                    switch (argv[i][j+1]){
                        case '0': case '1': case '2': case '3': case '4': 
                        case '5': case '6': case '7': case '8': case '9':
                            appendNum(ops,nxt_num);
                            appendOperator(ops,'*');
                            break;
                        default:
                            break;
                     }
                    
                }  
                
                break;  
            
            case ' ':
                if (num_strt) num_strt = 2;
                break;
                
            default:    
                preconError(__FILE__,__func__,CHR_ERR);
                break;   
        }
    }
    
    if (xtra_prc != 0) preconError(__FILE__,__func__,OPN_ERR);;

    if (num_strt && !lst_var) appendNum(ops,nxt_num);
    if (ops->bck->o != 'i' && ops->bck->o != 'x') 
        preconError(__FILE__,__func__,OPR_ERR); 
    return ops;
}



/* 
 * HELPER FUNCTION: makePrcList (OpList L)
 *      Finds index of operation with highest prc
 *
 * RETURNS: index of highest prc operation
 *
 * PRECONDITIONS:
 *      (1) L must contain at least one operation
 */
int getHighestPrc(OpList L){
    if (!L) nullError(__FILE__,__func__);
    if (L->len < 1) preconError(__FILE__,__func__,ARG_ERR);

    int toRet = 1;
    for (int i = 1; i < L->len; i+=2){
        Operation curr = toOp(L,i);
        if (curr->prc > toOp(L,toRet)->prc) toRet = i; 
    }
    return toRet;
}


/* 
 * FUNCTION: setAllOperands(OpList L)
 *      Sets operands of all operations in L in the order specified by 
 *      makePrcList. When operands are added to an operation, they are 
 *      removed from L, thus each time an operand is added the prcList 
 *      must be recalculated. When finished L will contain only one 
 *      operation. 
 *
 * RETURNS: void
 *
 * PRECONDITIONS:
 *      (1) L must contain at least one operation and two operands
 */
void setAllOperands(OpList L){
    if (!L) nullError(__FILE__,__func__);
    if (L->len < 3) preconError(__FILE__,__func__,ARG_ERR);
        
    while(L->len != 1) setOperands(L,getHighestPrc(L));
}

/* 
 * FUNCTION: evalList(OpList L)
 *      If a list contains one operation, it is evaluated. Otherwise 
 *      setAllOperands is called and then the function is evaluated to a
 *      double value.  
 *
 * RETURNS: double representation of result of L's operation(s)
 *
 * PRECONDITIONS:
 *      (1) L must contain at least one operation 
 */
double evalList(OpList L){
    if (!L) nullError(__FILE__,__func__);
    if (L->len == 0) preconError(__FILE__,__func__,ARG_ERR);

    if (L->len > 1) setAllOperands(L);
    return evalOp(L->frt);
}

/* 
 * HELPER FUNCTION: solveVarSwitched(OpList L)
 *      Some operations result in a tryVal and tryEval that grow in opposite
 *      directions (e.i. one gets larger positively as the other gets larger
 *      negetively). This function is identical to solveVal(), except that
 *      the comparison operation is switched to prevent an infinite loop. 
 *
 * RETURNS: double representation of estimate for var value
 *
 * PRECONDITIONS:
 *      (1) L must contain at least one operation 
 */
double solveVarSwitched(OpList L){
    if (!L) nullError(__FILE__,__func__);
    if (L->len == 0) preconError(__FILE__,__func__,ARG_ERR);
    
    if(L->len > 1) setAllOperands(L);

    
    double tryVal = ((MAX_VAR_VAL + MIN_VAR_VAL)/2);
    double tryEval = evalOp(L->frt,tryVal);
    double eqlVal = evalOp(L->eql);
    
    double min = MIN_VAR_VAL;
    double max = MAX_VAR_VAL;
    while (tryEval != eqlVal){
        tryEval = evalOp(L->frt,tryVal);
        if (tryEval > eqlVal){
            min = tryVal;
            tryVal = ((min + max)/2); 
        }else{
            max = tryVal;
            tryVal = ((min + max)/2);
        }
    }    
    return tryVal;
}



/* 
 * FUNCTION: solveVar(OpList L)
 *      Solves for variable by searching for correct value with binary
 *      search. Max and Min values for this function are defined in the 
 *      header of this module
 *
 * RETURNS: double representation of estimate for var value
 *
 * PRECONDITIONS:
 *      (1) L must contain at least one operation 
 */
double solveVar(OpList L){
    if (!L) nullError(__FILE__,__func__);
    if (L->len == 0) preconError(__FILE__,__func__,ARG_ERR);
    
    if(L->len > 1) setAllOperands(L);

    
    double tryVal = ((MAX_VAR_VAL + MIN_VAR_VAL)/2);
    double tryEval = evalOp(L->frt,tryVal);
    double eqlVal = evalOp(L->eql);
    
    double lst_eval = tryEval;
    double lst_tryV = tryVal;
    
    double min = MIN_VAR_VAL;
    double max = MAX_VAR_VAL;
    while ( tryEval != eqlVal){
        tryEval = evalOp(L->frt,tryVal);
        if (tryEval < eqlVal){
            min = tryVal;
            tryVal = ((min + max)/2); 
            
            if (lst_eval > tryEval && lst_tryV < tryVal) return solveVarSwitched(L);
        }else{
            max = tryVal;
            tryVal = ((min + max)/2);
            if (lst_eval < tryEval && lst_tryV > tryVal) return solveVarSwitched(L); 
        }

    }
    
    
    
    return tryVal;
}
