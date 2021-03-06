#ifndef CYCLE_H
#define CYCLE_h
#include "cell.h"
#include <stdio.h>

void make_a_cycle_rewrite_struct(Matrix *matrix, Matrix *matrix, int maxRow, int maxColumn, char neighbourType);

void add_neighbourhood_parametr(Matrix* mx, int maxRow, int maxColumn, char neighbourType, int row, int column);

#endif
