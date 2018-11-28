#ifndef LEXICAL
#define LEXICAL

/*
	 table - ����� �������
	 numberInTable - ����� ������� � �������
*/
typedef struct lex {
	int table;
	int numberInTable;
}lex;

/*
	table - ��������� �� ����������� �������.
	size - ������ �������, ��������� ��� ������� ����� �������.
	word_size - ������������ ������ ������ ����� � �������,
		��� �������, ��� ������ ����� ���� ��������� ��������, ��������� ��������
		�� �� ������ ������ � �������.
	table_number - ����� ������� ��� �������� ������.
*/
typedef struct tabl {
	char *table;
	int size;
	int word_size;
	int table_number;
}tabl;

void clear();
void add();
void makelex();
void gc();
void lex_analyzer();
void makeInternalRep(int, int);
void delimiterParser();
void printTable(tabl);
int getLexNumber();
int lexManager();
int isConstant();
int isLegalId();
int look(tabl);
int putl(tabl*);
void tabl_init();

/*
	Globals
*/
tabl TW;
tabl TD;
tabl TID;
tabl TNUM;

#endif // !LEXICAL
