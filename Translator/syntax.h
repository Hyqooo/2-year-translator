#pragma once
#ifndef SYNTAX_H
#define SYNTAX_H

int syntax_manager();
int dec();
int isId();
int progName();
int prog();
int decList();
int eq(char *);
int stmtList();
int functionList();
void getLex();
char* find();

#endif // !SYNTAX_H
