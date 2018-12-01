#ifndef TRANSLATOR
#define TRANSLATOR

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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

void error(char *);
int parseInt();

#endif // ! TRANSLATOR