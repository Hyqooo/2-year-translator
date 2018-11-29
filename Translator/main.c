#include "lexical.h"
#include "translator.h"

char buf[BUFFER_SIZE];
char ch;
int d;
int buff_index = 0;

extern lex cur_lex;

FILE *input = NULL;
FILE *output = NULL;

void main(int argc, char *argv[]) {
	// Lexical analysis
	lexManager();
//	syntax_manager();

	printf("\n");
	system("PAUSE");
}

#pragma region UTILITY

int parseInt() {
	char c;
	int number = 0;
	int length;
	length = strlen(buf);

	for (int i = 0; i < length; i++) {
		c = buf[i];
		if (c == '\0')
			continue;

		number += (c - '0') * (int)pow(10, length - i - 1);
	}

	return number;
}

void error(int error) {
	switch (error) {
	case 0:
		printf("\nProhibited lexeme is found.\n");
		break;
	case 1:
		printf("\nSyntax error.\n");
		break;
	}
	system("PAUSE");
	exit(0);
}

#pragma endregion