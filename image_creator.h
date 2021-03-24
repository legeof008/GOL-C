#ifndef _IMAGE_CREATOR_H_
#define _IMAGE_CREATOR_H_

#include "cell.h"

// Zapisuje matrix do pliku w postaci bitmapy; zwraca 0, gdy udalo sie zapisac obraz; zwraca -1, gdy nie udalo sie zapisac obrazu
int save_as_bitmap(FILE* file, Matrix *matrix, int scale);

#endif // _IMAGE_CREATOR_H_
