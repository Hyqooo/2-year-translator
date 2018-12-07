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
	char type[MAX_ID_SIZE];
}function;

void syntax_manager();
int isId();
int prog();
int decList();
int eq(char *);
int ipop();
int operationSign();
int assign();
int isNum();
void idList();
void expression();
void stmtList();
void stmt();
void functionList();
void dec();
void getBack();
void name();
void ipush(int);
void getLex();
void tpush(char *);
void checkOp();
void for_loop();
void argList();
void func();
void func_call();
void body();
char* find();
char *typeOfConstant();
char* tpop();

#endif // !SYNTAX_H
