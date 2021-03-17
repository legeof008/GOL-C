#ifndef _CYCLE_H_
#define _CYCLE_H_

#include "cell.h"

void add_neighbourhood_parametr(Matrix* mx, int maxRow, int maxColumn, char neighbourType, int row, int column, char connection);

void make_a_cycle_rewrite_struct(Matrix* mx, Matrix* nx, int maxRow, int maxColumn, char neighbourType);

#endif // _CYCLE_H_
