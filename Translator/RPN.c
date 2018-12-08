#include "RPN.h"
#include "translator.h"

char operatorStack[SIZE_OF_OP_STACK][4];
int opStackPointer = 0;

char buffer[SIZE_OF_SINGLE_OP];
int pointToWriteBuff = 0;
int pointToNextElement = 0;

char element[MAX_ID_SIZE];

char internalRepresentation[NUMBER_OF_OP][SIZE_OF_SINGLE_OP];
int countOperators = 0;
static int pointToWriteRep = 0;

// Operands and operators separated by space
void arithmeticParser() {
	char *toRep;
	while (1) {
		// Get next element of expression
		restoreElement();
		strcpy(element, getNextEl());
		if (strlen(element) == 0)
			break;

		// Element is number or id
		if (isNumOrId(element))
			addToFinalRep(element);
		if (isSignOfOp(element)) {
			// Element at the top of the stack has greater or equal precedence and element not equal '('
			while (strcmp(lookTopOfStack(), "(") && isGrtrOrEqPrecedence(element, lookTopOfStack())) {
				// Pop elements from stack to final representation until it '('
				toRep = popOutOpStack();
				if (!toRep) break;
				if (strcmp(toRep, "("))
					addToFinalRep(toRep);
			}
			pushToOpStack(element);
		}

		// Element is left bracket '('
		if (!strcmp(element, "("))
			pushToOpStack(element);
		// Element is right bracket ')'
		if (!strcmp(element, ")")) {
			// Until element at top of the stack not a left bracket
			while (strcmp(lookTopOfStack(), "(")) {
				toRep = popOutOpStack();
				if (!toRep) break;
				if (strcmp(toRep, "("))
					addToFinalRep(toRep);
			}
			popOutOpStack();
		}
	}

	while (opStackPointer) {
		toRep = popOutOpStack();
		if (!toRep) break;
		if (strcmp(toRep, "("))
			addToFinalRep(toRep);
	}
}

void addLexToBuffer(char *lex) {
	char *toWrite = buffer + pointToWriteBuff * sizeof(char);
	strcpy(toWrite, lex);
	pointToWriteBuff += strlen(lex) + 1;
	buffer[pointToWriteBuff - 1] = ' ';
}

void restore() {
	for (int i = 0; i < SIZE_OF_SINGLE_OP; i++)
		buffer[i] = '\0';
	pointToWriteBuff = 0;
	pointToWriteRep = 0;
	opStackPointer = 0;
	pointToNextElement = 0;
}

void addToFinalRep(char *lex) {
	char *toWrite = internalRepresentation[countOperators] + pointToWriteRep * sizeof(char);
	strcpy(toWrite, lex);
	pointToWriteRep += strlen(lex) + 1;
	internalRepresentation[countOperators][pointToWriteRep - 1] = ' ';
}

char *getNextEl() {
	char *c = &buffer[pointToNextElement++];
	for (int i = 0; *c != ' '; c++, i++, pointToNextElement++) {
		if (*c == '\0') break;
		element[i] = *c;
	}

	return element;
}

void restoreElement() {
	char *c = element;
	for (int i = 0; *c != '\0'; c++, i++)
		element[i] = '\0';
}

int isSignOfOp(char *str) {
	if (*str == '+' || *str == '-' || *str == '*' || !strcmp(str, "DIV"))
		return 1;

	return 0;
}

int isNumOrId(char *str) {
	if (isdigit(*str))
		return 1;

	if (strcmp("DIV", str) && !isSignOfOp(str) && strcmp("(", str) && strcmp(")", str))
		return 1;

	return 0;
}

void pushToOpStack(char *el) {
	strcpy(operatorStack[opStackPointer], el);
	opStackPointer++;
}

char *popOutOpStack() {
	if (!opStackPointer) return NULL;
	opStackPointer--;
	return operatorStack[opStackPointer];
}

char *lookTopOfStack() {
	return operatorStack[opStackPointer - 1];
}

// Greater of equal precedence
int isGrtrOrEqPrecedence(char *el, char *tOfStack) {
	if (!strcmp("DIV", el) || !strcmp("*", el)) {
		if (!strcmp("DIV", tOfStack) || !strcmp("*", tOfStack))
			return 1;
		else
			return 0;
	}else {
		if (!strcmp("+", tOfStack) || !strcmp("-", tOfStack))
			return 0;
		else
			return 1;
	}

}

void assignRPN() {
	// Adds left-side variable to representation
	addToFinalRep(getNextEl());

	// Arithmetic expression analysis
	arithmeticParser();

	// Adds ':=' to the end
	addToFinalRep(":=");

	countOperators++;
}

void readParser() {
	while (1) {
		// Get element
		restoreElement();
		strcpy(element, getNextEl());
		if (strlen(element) == 0)
			break;

		addToFinalRep(element);
		addToFinalRep("READ");
		countOperators++;
		pointToWriteRep = 0;
	}
}

void writeParser() {
	while (1) {
		// Get element
		restoreElement();
		strcpy(element, getNextEl());
		if (strlen(element) == 0)
			break;

		addToFinalRep(element);
		addToFinalRep("WRITE");
		countOperators++;
		pointToWriteRep = 0;
	}
}