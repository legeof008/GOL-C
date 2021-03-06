#ifndef _IMAGE_CREATOR_H_
#define _IMAGE_CREATOR_H_

#include "cell.h"

// Zapisuje matrix do pliku w postaci bitmapy
void save_as_bitmap(char *path, Matrix *matrix);

// [DEBUG] Czyta naglowek pliku o sciezce bitmapPath i zapiuje go do pliku o sciezce outputPath
void read_bitmap(char *bitmapPath, char *outputPath);

#endif // _IMAGE_CREATOR_H_
