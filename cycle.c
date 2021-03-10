#include "cycle.h"

#include <stdio.h>
#include "cell.h"

void add_neighbourhood_parametr(Matrix* mx, int maxRow, int maxColumn, char neighbourType, int row, int column) {
	Cell* dmx = mx->data;
	int i;
	if (row == 0) {
		if (column == 0) {
			(dmx + row * mx->c + column + 1)->neighbor++;
			(dmx + row + 1 * mx->c + column)->neighbor++;
			if (neighbourType == 2) {			//przekatna dla trybu 8 sasiadow
				(dmx + row + 1 * mx->c + column + 1)->neighbor++;		//przekatna prawo gora
			}
		}
		if (column > 0 && column < maxColumn) {
			if (neighbourType == 2) {
				for (i = -1; i < 2; i++)
					(dmx + row + 1 * mx->c + column + i)->neighbor++;
			}
			else
			{
				(dmx + row + 1 * mx->c + column)->neighbor++;
			}
			(dmx + row * mx->c + column + 1)->neighbor++;
			(dmx + row * mx->c + column - 1)->neighbor++;
		}
		if (column == maxColumn) {
			(dmx + row + 1 * mx->c + column)->neighbor++;
			(dmx + row * mx->c + column - 1)->neighbor++;
			if (neighbourType == 2)
				(dmx + row + 1 * mx->c + column - 1)->neighbor++;
		}
	}
	if (row != 0 && row != maxRow) {
		if (column == 0) {
			if (neighbourType == 2) {
				for (i = row - 1; i < row + 2; i++) {
					(dmx + i * mx->c + column + 1)->neighbor++;			//prwao blok
				}
			}
			else {
				(dmx + row * mx->c + column + 1)->neighbor++;
			}
			(dmx + row + 1 * mx->c + column)->neighbor++;			//gora srodek
			(dmx + row - 1 * mx->c + column)->neighbor++;			//dol srodek
		}
		if (column > 0 && column < maxColumn) {
			if (neighbourType == 2) {
				for (i = row - 1; i < row + 2; i++) {
					(dmx + i * mx->c + column - 1)->neighbor++;			//lewo blok
					(dmx + i * mx->c + column + 1)->neighbor++;			//prwao blok
				}
			}
			(dmx + row * mx->c + column + 1)->neighbor++;			//prawo srodek
			(dmx + row * mx->c + column - 1)->neighbor++;			//lewo srodek
			(dmx + row + 1 * mx->c + column)->neighbor++;			//gora srodek
			(dmx + row - 1 * mx->c + column)->neighbor++;			//dol srodek
		}
		if (column == maxColumn) {
			if (neighbourType == 2) {
				for (i = row - 1; i < row + 2; i++) {
					(dmx + i * mx->c + column - 1)->neighbor++;			//lewo blok
				}
			}
			(dmx + row * mx->c + column - 1)->neighbor++;
			(dmx + row + 1 * mx->c + column)->neighbor++;			//gora srodek
			(dmx + row - 1 * mx->c + column)->neighbor++;			//dol srodek
		}
	}
	if (row == maxRow) {
		if (column == 0) {
			if (neighbourType == 2)
				(dmx + row - 1 * mx->c + column + 1)->neighbor++;
			(dmx + row * mx->c + column + 1)->neighbor++;
			(dmx + row - 1 * mx->c + column)->neighbor++;
		}
		if (column > 0 && column < maxColumn) {
			if (neighbourType == 2) {
				for (i = -1; i < 2; i++)
					(dmx + row - 1 * mx->c + column + i)->neighbor++;
			}
			else
			{
				(dmx + row * mx->c + column - 1)->neighbor++;
			}
			(dmx + row * mx->c + column + 1)->neighbor++;
			(dmx + row * mx->c + column - 1)->neighbor++;
		}
		if (column == maxColumn) {
			if (neighbourType == 2)
				(dmx + row - 1 * mx->c + column - 1)->neighbor++;
			(dmx + row * mx->c + column - 1)->neighbor++;
			(dmx + row - 1 * mx->c + column)->neighbor++;
		}
	}
}

void make_a_cycle_rewrite_struct(Matrix* mx, Matrix* nx, int maxRow, int maxColumn, char neighbourType) {
	Cell* dmx = mx->data;
	Cell* dmc = nx->data;
	int i;
	int j;
	for (i = 0; i < maxRow; i++) {
		for (j = 0; j < maxColumn; j++) {
			if ((dmx + i * mx->c + j)->type == 2)		//przepisywanie scian
				(dmc + i * mx->c + j)->type = 2;
			if ((dmx + i * mx->c + j)->type == 1) {
				if ((dmx + i * mx->c + j)->neighbor == 2 || (dmx + i * mx->c + j)->neighbor == 3) {		//przepisuje punkt zywy do nowej strukturze
					(dmc + i * nx->c + j)->type = 1;
					add_neighbourhood_parametr(nx, maxRow, maxColumn, neighbourType, i, j);		//dodaje parametr sasiad
				}
			}
			if ((dmx + i * mx->c + j)->type == 0 && (dmx + i * mx->c + j)->neighbor == 3) {
				(dmc + i * nx->c + j)->type = 1;
				add_neighbourhood_parametr(nx, maxRow, maxColumn, neighbourType, i, j);
			};
		}
	}
}















