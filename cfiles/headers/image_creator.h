#ifndef _IMAGE_CREATOR_H_
#define _IMAGE_CREATOR_H_

#include <png.h>
#include "cell.h"

int width;
int height;
png_byte colorType;
png_byte bitDepth;
png_bytep *rowPointers;
png_structp png;
png_infop info;

// Zapisuje matrix do pliku png, zwraca ujemną wartość, jeżeli nie udało się zapisać obrazu
int write_png_file(FILE *fp);

// Przetwarza dana matrix, aby później móc utowrzyć z niej plik png
void process_png_file(Matrix *matrix, int scale);

// Zapisuje matrix do pliku w postaci bitmapy; zwraca 0, gdy udalo sie zapisac obraz; zwraca -1, gdy nie udalo sie zapisac obrazu
void save_as_bitmap(FILE* file, Matrix *matrix, int scale);

#endif // _IMAGE_CREATOR_H_
