#include "RPN.h"
#include "translator.h"

char buffer[SIZE_OF_SINGLE_OP];
char internalRepresentation[NUMBER_OF_OP][SIZE_OF_SINGLE_OP];
int countOperators = 0;
int pointToWrite = 0;

// Operands and operators separated by space
void arithmeticParser() {
	
}

void addLexToBuffer(char *lex) {
	char *toWrite = buffer + pointToWrite * sizeof(char);
	strcpy(toWrite, lex);
	pointToWrite += strlen(lex) + 1;
	buffer[pointToWrite - 1] = ' ';
}

void restoreBuffer() {
	for (int i = 0; i < SIZE_OF_SINGLE_OP; i++)
		buffer[i] = '\0';
	pointToWrite = 0;
}