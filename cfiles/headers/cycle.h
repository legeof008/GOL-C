#ifndef _CYCLE_H_
#define _CYCLE_H_
#include "cell.h"

void count_neighbours_4(Matrix* mx, int connection);
void make_a_cycle_rewrite_struct_4(Matrix* mx, Matrix* nx, int maxRow, int maxColumn, int connection);

void count_neighbours_8(Matrix* mx, int connection);
void make_a_cycle_rewrite_struct_8(Matrix* mx, Matrix* nx, int maxRow, int maxColumn, int connection);

#endif // _CYCLE_H_
