#ifndef _IMAGE_CREATOR_H_
#define _IMAGE_CREATOR_H_

#include "cell.h"

// Zapisuje matrix do pliku w postaci bitmapy; zwraca 0, gdy udalo sie zapisac obraz; zwraca -1, gdy nie udalo sie zapisac obrazu
int save_as_bitmap(char *path, Matrix *matrix, int scale);

// [DEBUG] Czyta naglowek pliku o sciezce bitmapPath i zapiuje go do pliku o sciezce outputPath
void read_bitmap(char *bitmapPath, char *outputPath);

#endif // _IMAGE_CREATOR_H_
