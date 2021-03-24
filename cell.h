#ifndef _CELL_MX_H_
#define _CELL_MX_H_

#include <stdio.h>

#pragma pack(push, 1)
typedef struct{
	char type; // 0 - martwa; 1 - żywa; 2 - ściana;
	unsigned char R;
	unsigned char G;
	unsigned char B;
	char neighbor; // miedzy 0 a 8 sąsiadów
	char padding [3];
}Cell;
typedef struct {
	
	Cell* data;
	int r,c; // wiersze, kolumny
	
}Matrix;
#pragma pack(pop)

Matrix *mx_alloc(int r, int c); // alokuje pamięć na Matrix

Matrix* mx_read_from_file(FILE *f) ; // Czyta z pliku

Cell mx_get_cell(Matrix *mx, int r, int c); 

void mx_print(Matrix *mx); // wypisuje jak macierz poglądowo

void mx_cpy(Matrix *src, Matrix *dest); //Kopiowanie macierzy z zerowaniem src

char mx_get_single_val(Matrix *mx, int r, int c, char type); // wydobycie pojedynczej cechy komórki, t- type, n - sąsiad

int mx_put_val(Matrix *mx, int r, int c, int num, ...); /* wieloargumentowa funkcja, NIE dawać więcej niż 3 argumenty po num*/

#endif

