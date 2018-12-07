#pragma once
#ifndef RPN_H
#define RPN_H

#define SIZE_OF_SINGLE_OP 128
#define NUMBER_OF_OP 124

void parserArithmExp();
void addLexToBuffer(char *);
void restoreBuffer();


#endif // !RPN_H

