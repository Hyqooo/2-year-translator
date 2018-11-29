#pragma once
#ifndef SYNTAX_H
#define SYNTAX_H

#define STACK_SIZE 20

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
char* find();

#endif // !SYNTAX_H
