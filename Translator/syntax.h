#pragma once
#ifndef SYNTAX_H
#define SYNTAX_H

#define STACK_SIZE 20
#define MAX_AMOUNT_OF_FUNCTIONS 10

typedef struct function {
	char name[MAX_ID_SIZE];
	char *variables[SIZE_OF_TID_TABLE];
	int sizeVar;
	int sizeRec;
	record varDeclarations[SIZE_OF_TID_TABLE];
}function;

int syntax_manager();
int dec();
int isId();
int progName();
int prog();
int decList();
int eq(char *);
int stmtList();
int functionList();
int ipop();
void ipush(int);
void getLex();
void tpush(char *);
void checkOp();
char* find();
char *typeOfConstant();
char* tpop();

#endif // !SYNTAX_H
