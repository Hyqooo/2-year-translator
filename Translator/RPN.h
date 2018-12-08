#pragma once
#ifndef RPN_H
#define RPN_H

#define SIZE_OF_SINGLE_OP 128
#define NUMBER_OF_OP 124
#define SIZE_OF_OP_STACK 20

typedef struct state {
	int line;
	int position;
}state_t;

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
void forParser();
char *popOutOpStack();
char *lookTopOfStack();
int isGrtrOrEqPrecedence(char *, char *);
int isSignOfOp(char *);
int isNumOrId(char *);

void fillGaps(state_t, state_t);

state_t popStateStack();
void pushToStateStack(state_t);

char *parseIntToString(int);

char *popVarForStack();
void pushVarForStack(char *);


#endif // !RPN_H

