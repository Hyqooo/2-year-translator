/*
	������������ ���� �����������. �������� �������, ��������� � �������� �������
*/

#ifndef TRANSLATOR
#define TRANSLATOR

#define MAX_ID_SIZE 64
#define MAX_NUM_SIZE 15
#define SIZE_OF_TW_TABLE 20
#define SIZE_OF_TD_TABLE 13
#define SIZE_OF_TNUM_TABLE 64
#define SIZE_OF_TID_TABLE 128
#define BUFFER_SIZE 64
#define WORD_SIZE_IN_TABLE_TW 10
#define WORD_SIZE_IN_TABLE_TD 4
#define NOT_FOUND -1

//	table - ��������� �� ����������� �������.
//	size - ������ �������, ��������� ��� ������� ����� �������.
//	word_size - ������������ ������ ������ ����� � �������,
//		��� �������, ��� ������ ����� ���� ��������� ��������, ��������� ��������
//		�� �� ������ ������ � �������.
//	table_number - ����� ������� ��� �������� ������.

typedef struct tabl {
	char *table;
	int size;
	int word_size;
	int table_number;
}tabl;

/*
	���������
*/
void clear();
void add();
void makelex();
void gc();
void tabl_init();
void tabl_free();
void error();
void lex_analyzer();
void makeInternalRep();
void delimiterParser();
void printTable();
int isConstant();
int isLegalId();
int look(tabl t);
int putl(tabl* t);
int putnum();

/*
	�������
*/
// ������� ��������� ����.
char TW_char[][WORD_SIZE_IN_TABLE_TW] = {
	"PROGRAM",
	"VAR",
	"BEGIN",
	"END",
	"INTEGER",
	"REAL",
	"READ",
	"WRITE",
	"FOR",
	"DO",
	"TO",
	"FUNCTION",
	"RETURN",
	"IF",
	"THEN",
	"ELSE IF",
	"OR",
	"AND",
	"DIV",
	"END."
};

// ������� ������������.
char TD_char[][WORD_SIZE_IN_TABLE_TD] = {
	",",
	";",
	"+",
	"-",
	"*",
	":=",
	"==",
	"!=",
	">",
	"<",
	":",
	"(",
	")"
};

// ������� ��������������� � ��������� �������� ��������������.
char **TID_char, **TNUM_char;
#endif // ! TRANSLATOR