#ifndef _IMAGE_CREATOR_H_
#define _IMAGE_CREATOR_H_

#include "cell.h"

int width;
int height;
png_byte colorType;
png_byte bitDepth;
png_bytep *rowPointers;
png_structp png;
png_infop info;

// Zapisuje matrix do pliku png
int write_png_file(char *filename);

// Przetwarza dana matrix, zwraca 0 gdy przetworzyl poprawnie, -1 gdy skala jest nieprawidlowa
int process_png_file(Matrix *matrix, int scale);

// Zapisuje matrix do pliku w postaci bitmapy; zwraca 0, gdy udalo sie zapisac obraz; zwraca -1, gdy nie udalo sie zapisac obrazu
int save_as_bitmap(FILE* file, Matrix *matrix, int scale);

#endif // _IMAGE_CREATOR_H_
