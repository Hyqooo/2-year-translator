#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "translator.h"

char buf[BUFFER_SIZE];
char ch;
int d;
int buff_index = 0;

tabl TW;
tabl TD;
tabl TID;
tabl TNUM;

FILE *input = NULL;
FILE *output = NULL;

void main(int argc, char *argv[]) {
	// Lexical analysis
	lexManager();


	printf("\n");
	system("PAUSE");
}
#pragma region LEXICAL ANALYZER

void lexManager() {
	tabl_init();

	if ((input = fopen("D:\\source.txt", "r")) == NULL)
		return;

	if ((output = fopen("D:\\lex_analysis", "w")) == NULL)
		return;

	lex_analyzer();

	printf("\nIdentifiers:\n");
	printTable(TID);
	printf("Constants:\n");
	printTable(TNUM);

	fclose(input);
}

void printTable(tabl t) {
	for (int i = 0; i < t.size; i++) {
		printf("%d)%s\n", i + 1, t.table + i * t.word_size);
	}
}

void lex_analyzer() {
	while (1) {
		gc();
		if (!isalnum(ch)) {
			if (ch == '.') {
				add();
				gc();
				if (ch == '.') 
					error();
				else
					continue;
			}
			makelex();
			delimiterParser();
			if (ch == EOF) break;
			continue;
		}
		add();
	}
}

void delimiterParser() {
	int indexInTable;
	char firstDel[2];

	if (ch == EOF) return;

	firstDel[0] = ch;
	firstDel[1] = '\0';

	clear();

	if (isspace(ch)) {
		printf("%c", ch);
		return;
	}

	add();
	gc();
	add();
	if ((indexInTable = look(TD)) == NOT_FOUND) {
		clear();
		strcpy(buf, firstDel);
		if ((indexInTable = look(TD)) == NOT_FOUND)
			error();
		fseek(input, -1, SEEK_CUR);
	}

	makeInternalRep(TD.table_number, indexInTable);

	clear();
}

void clear() {
	for (int i = 0; i < buff_index; i++) {
		buf[i] = '\0';
	}

	buff_index = 0;
}

void add() {
	buf[buff_index] = ch;
	buf[buff_index + 1] = '\0';
	buff_index++;
}

void makelex() {
	if (buff_index == 0)
		return;

	int indexInTable;
	if ((indexInTable = look(TW)) != NOT_FOUND) {
		makeInternalRep(TW.table_number, indexInTable);
	}
	else if (isConstant()) {
		if ((indexInTable = look(TNUM)) == NOT_FOUND) {
			indexInTable = putl(&TNUM);
		}
		makeInternalRep(TNUM.table_number, indexInTable);
	}
	else {
		if (!isLegalId())
			error();
		if ((indexInTable = look(TID)) == NOT_FOUND) {
			indexInTable = putl(&TID);
		}
		makeInternalRep(TID.table_number, indexInTable);
	}
}

void makeInternalRep(int tableNum, int numberInTable) {
	fprintf(stdout, "<%d, %d>", tableNum, numberInTable);
}

int isConstant() {
	int count = 0;
	for (int i = 0; i < buff_index; i++) {
		if (!isdigit(buf[i])) {
			if (buf[i] != '.')
				return 0;
			count++;
			if (count > 1)
				return 0;
		}
	}

	return 1;
}

void gc() {
	ch = fgetc(input);
}

int look(tabl t) {
	for (int i = 0; i < t.size; i++) {
		if (!strcmp(buf, t.table + i * t.word_size)) {
			return i;
		}
	}

	return NOT_FOUND;
}

int putl(tabl* t) {
	int index = t->size++;
	strcpy(t->table + index * t->word_size, buf);
	return t->size - 1;
}

int isLegalId() {
	return isalpha(buf[0]) ? 1 : 0;
}



#pragma endregion

#pragma region SYNTAX ANALYZER

void syntax_manager() {

}

#pragma endregion

#pragma region UTILITY

void error() {
	printf("\nProhibited lexeme is found.\n");
	system("PAUSE");
	exit(0);
}

void tabl_init() {

	TW.table = (char *)TW_char;
	TW.size = SIZE_OF_TW_TABLE;
	TW.word_size = WORD_SIZE_IN_TABLE_TW;
	TW.table_number = 1;

	TD.table = (char *)TD_char;
	TD.size = SIZE_OF_TD_TABLE;
	TD.word_size = WORD_SIZE_IN_TABLE_TD;
	TD.table_number = 2;

	TNUM_char = (char**)malloc(sizeof(char*) * SIZE_OF_TNUM_TABLE);
	for (int i = 0; i < SIZE_OF_TNUM_TABLE; i++) {
		TNUM_char[i] = (char*)malloc(sizeof(char) * MAX_NUM_SIZE);
	}

	TNUM.table = (char *)TNUM_char;
	TNUM.size = 0;
	TNUM.word_size = MAX_NUM_SIZE;
	TNUM.table_number = 3;

	TID_char = (char**)malloc(sizeof(char*) * SIZE_OF_TID_TABLE);
	for (int i = 0; i < SIZE_OF_TID_TABLE; i++) {
		TID_char[i] = (char*)malloc(sizeof(char) * MAX_ID_SIZE);
	}

	TID.table = (char *)TID_char;
	TID.size = 0;
	TID.word_size = MAX_ID_SIZE;
	TID.table_number = 4;
}

#pragma endregion