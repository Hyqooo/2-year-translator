#pragma once
#ifndef RPN_H
#define RPN_H

#define SIZE_OF_SINGLE_OP 128
#define NUMBER_OF_OP 124
#define SIZE_OF_OP_STACK 20

void arithmeticParser();
void addLexToBuffer(char *);
void restore();
void addLexToBuffer(char *);
char *getNextEl();
void addToFinalRep(char *);
void restoreElement();
void pushToOpStack(char *);
void assignRPN();
void readParser();
void writeParser();
char *popOutOpStack();
char *lookTopOfStack();
int isGrtrOrEqPrecedence(char *, char *);
int isSignOfOp(char *);
int isNumOrId(char *);


#endif // !RPN_H

