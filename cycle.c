#include "cycle.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>	// TODO: srand powinien byc w mainie
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

// Sumuje 2 uchary i ogranicza wartosc sumy do 255
unsigned char add_clamped(unsigned char f, unsigned char s)
{
	if (f + s > 255)
		return 255;
	else
		return f + s;
}

// Zwraca komorke z polaczonym kolorem z 2 rodzicow
Cell combine_colors(Cell first, Cell second)
{
	srand(time(NULL));	// TODO: srand powinien byc w mainie

	// Wybieranie glownego rodzica
	int p = rand() % 2;		// 0 -> 1 jest glowny; 1 -> 2 jest glowny

	Cell mainParent = p ? first : second;
	Cell secondParent = p ? second : first;

	Cell newCell =
	{
		.type = 1,
		.R = 0,
		.G = 0,
		.B = 0,
		.neighbor = 0
	};

	// Losowanie numerow bitow, ktore zostana dodane
	char bitNumbers[3];

	bitNumbers[0] = rand() % 24;
	do
	{
		bitNumbers[1] = rand() % 24;
	} while (bitNumbers[1] == bitNumbers[0]);
	do
	{
		bitNumbers[2] = rand() % 24;
	} while (bitNumbers[2] == bitNumbers[0] || bitNumbers[2] == bitNumbers[1]);

	// Tworzenie kolorow z wylosowanych numerow bitow

	for (int i = 0; i < 3; i++)
	{
		if (bitNumbers[i] < 8)
			newCell.B += (1 << bitNumbers[i]) & secondParent.B;
		else if (bitNumbers[i] < 16)
			newCell.G += (1 << (bitNumbers[i] % 8)) & secondParent.G;
		else
			newCell.R += (1 << (bitNumbers[i] % 8)) & secondParent.R;
	}

	// Dodawanie kolorow glownego rodzica
	newCell.R = add_clamped(mainParent.R, newCell.R);
	newCell.G = add_clamped(mainParent.G, newCell.G);
	newCell.B = add_clamped(mainParent.B, newCell.B);

	return newCell;
}














