#include "cycle.h"

#include <stdio.h>
#include <stdlib.h>
#include "cell.h"

#pragma warning(disable:4700)

// Sumuje 2 uchary i ogranicza wartosc sumy do 255
unsigned char add_clamped(unsigned char f, unsigned char s)
{
	if (f + s > 255)
		return 255;

	return f + s;
}

int clamp_index(int v, int s, int connection)
{
	if (connection == 1)
	{
		if (v < 0)
			return v + s;
		if (v >= s)
			return v - s;
	}
	else
	{
		if (v < 0)
			return 0;
		if (v > s)
			return s;
	}

	return v;
}

Cell* get_neighbours_4(Matrix* mx, int x, int y, int connection)
{
	Cell* neighbours = malloc(sizeof(Cell) * 3);
	int neighbourCounter = 0;
	int nx, ny;

	if (neighbours == NULL)
	{
		fprintf(stderr, "Nie udalo sie zarezerwowac pamieci na liste sasiadow");
		return NULL;
	}

	ny = clamp_index(y - 1, mx->r, connection);
	nx = x;
	if (mx_get_single_val(mx, ny, nx, 't') == 1)
		neighbours[neighbourCounter++] = mx_get_cell(mx, ny, nx);
	ny = clamp_index(y + 1, mx->r, connection);
	if (mx_get_single_val(mx, ny, nx, 't') == 1)
		neighbours[neighbourCounter++] = mx_get_cell(mx, ny, nx);
	ny = y;
	nx = clamp_index(x - 1, mx->c, connection);
	if (mx_get_single_val(mx, ny, nx, 't') == 1)
		neighbours[neighbourCounter++] = mx_get_cell(mx, ny, nx);
	nx = clamp_index(x + 1, mx->c, connection);
	if (mx_get_single_val(mx, ny, nx, 't') == 1)
	{
		if (neighbourCounter == 3)
		{
			fprintf(stderr, "Komorka(%d, %d) nie posiada 3 sasiadow (%d)\n", x, y, neighbourCounter);
			return NULL;
		}
		neighbours[neighbourCounter++] = mx_get_cell(mx, ny, nx);
	}

	if (neighbourCounter != 3)
	{
		fprintf(stderr, "Komorka(%d, %d) nie posiada 3 sasiadow (%d)\n", x, y, neighbourCounter);
		return NULL;
	}

	return neighbours;
}

// Zwraca komorke z polaczonym kolorem z 2 rodzicow
Cell create_cell_with_inherited_colors_4(Matrix* mx, int x, int y, int n, int connection)
{
	Cell* neighbours;

	Cell mainParent;
	Cell secondParent;

	Cell newCell =
	{
		.type = 1,
		.R = 0,
		.G = 0,
		.B = 0,
		.neighbor = n
	};

	int mainParentIndex;
	int secondParentIndex;

	int bitNumbers[3];

	
	neighbours = get_neighbours_4(mx, x, y, connection);

	if (neighbours == NULL)
	{
		fprintf(stderr, "Nie udalo sie znalezc sasiadow\n");
		return newCell;
	}

	// Losowanie indexow rodzicow
	mainParentIndex = rand() % 3;
	do
	{
		secondParentIndex = rand() % 3;
	} while (secondParentIndex == mainParentIndex);

	mainParent = neighbours[mainParentIndex];
	secondParent = neighbours[secondParentIndex];

	// Losowanie numerow bitow, ktore zostana dodane
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

	//printf("----\n%d %d %d \nmain %d %d %d\nsecond %d %d %d\n", newCell.R, newCell.G, newCell.B, mainParent.R, mainParent.G, mainParent.B, secondParent.R, secondParent.G, secondParent.B);
	//printf("%d %d %d\n", bitNumbers[0], bitNumbers[1], bitNumbers[2]);
	return newCell;
}

void add_neighbourhood_parametr_4(Matrix* mx, int row, int column, int connection)
{
	Cell* dmx = mx->data;
	int i;
	int maxRow = mx->r - 1;
	int maxColumn = mx->c - 1;

	if (row == 0)
	{
		if (column == 0)
		{
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + row * mx->c + (column + 1))->neighbor++;			// prawo
			if (connection == 1)		// Tryb zawijania
			{
				(dmx + row * mx->c + maxColumn)->neighbor++;		// lewo
				(dmx + maxRow * mx->c)->neighbor++;					// dol
			}
		}
		else if (column < maxColumn)
		{
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + row * mx->c + (column + 1))->neighbor++;			// lewo
			(dmx + row * mx->c + (column - 1))->neighbor++;			// prawo
			if (connection == 1)		// Tryb zawijania
				(dmx + column)->neighbor++;							// dol
		}
		else if (column == maxColumn)
		{
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + row * mx->c + (column - 1))->neighbor++;			// lewo
			if (connection == 1)		// Tryb zawijania
			{
				(dmx + row * mx->c)->neighbor++;					// prawo
				(dmx + maxRow * mx->c + column)->neighbor++;		// dol
			}
		}
		else
		{
			// TODO: Podany zly indeks kolumny
		}
	}
	else if (row < maxRow)
	{
		if (column == 0)
		{
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + row * mx->c + (column + 1))->neighbor++;			// prawo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
			if (connection == 1)		// Tryb zawijania
				(dmx + row * mx->c + maxColumn)->neighbor++;		// lewo
		}
		else if (column < maxColumn)
		{
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + row * mx->c + (column - 1))->neighbor++;			// lewo
			(dmx + row * mx->c + (column + 1))->neighbor++;			// prawo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
		}
		else if (column == maxColumn)
		{
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + row * mx->c + (column - 1))->neighbor++;			// lewo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
			if (connection == 1)		// Tryb zawijania
				(dmx + row * mx->c)->neighbor++;					// prawo
		}
		else
		{
			// TODO: Podany zly indeks kolumny
		}
	}
	else if (row == maxRow)
	{
		if (column == 0)
		{
			(dmx + row * mx->c + (column + 1))->neighbor++;			// prawo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
			if (connection == 1)		// Tryb zawijania
			{
				(dmx + row * mx->c + maxColumn)->neighbor++;		// lewo
				(dmx)->neighbor++;									// gora 
			}//HACK: zobaczyc czy to dziala
		}
		else if (column < maxColumn)
		{
			(dmx + row * mx->c + (column - 1))->neighbor++;			// lewo
			(dmx + row * mx->c + (column + 1))->neighbor++;			// prawo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
			if (connection == 1)		// Tryb zawijania
				(dmx + column)->neighbor++;							// gora
		}
		else if (column == maxColumn)
		{
			(dmx + row * mx->c + (column - 1))->neighbor++;			// lewo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
			if (connection == 1) 		// Tryb zawijania
			{
				(dmx + maxRow * mx->c)->neighbor++;					// prawo
				(dmx + column)->neighbor++;							// gora
			}
		}
		else
		{
			// TODO: Podany zly indeks kolumny
		}
	}
	else
	{
		// TODO: Podany zly indeks rzedu
	}
}

void count_neighbours_4(Matrix* mx, int connection)
{
	for (int i = 0; i < mx->r; i++)
		for (int j = 0; j < mx->c; j++)
		{
			if (mx_get_single_val(mx, i, j, 't') == 1)
				add_neighbourhood_parametr_4(mx, i, j, connection);
		}
}

void make_a_cycle_rewrite_struct_4(Matrix* mx, Matrix* nx, int maxRow, int maxColumn, int connection)
{
	Cell* dmx = mx->data;
	Cell* dnx = nx->data;
	int i;
	int j;

	for (i = 0; i <= maxRow; i++)
	{
		for (j = 0; j <= maxColumn; j++) 
		{
			if ((dmx + i * mx->c + j)->type == 2)		// sciana
			{		
				(dnx + i * mx->c + j)->type = 2;
			}
			else if ((dmx + i * mx->c + j)->type == 1)		// zywa
			{
				if ((dmx + i * mx->c + j)->neighbor == 2 || (dmx + i * mx->c + j)->neighbor == 3) 
				{		//przepisuje punkt zywy do nowej strukturze
					(dnx + i * nx->c + j)->type = 1;
					add_neighbourhood_parametr_4(nx, i, j, connection);		//dodaje parametr sasiad
					(dnx + i * mx->c + j)->R = (dmx + i * mx->c + j)->R;
					(dnx + i * mx->c + j)->G = (dmx + i * mx->c + j)->G;
					(dnx + i * mx->c + j)->B = (dmx + i * mx->c + j)->B;
				}
			}
			else if ((dmx + i * mx->c + j)->type == 0 && (dmx + i * mx->c + j)->neighbor == 3) 
			{
				dnx[i * mx->c + j] = create_cell_with_inherited_colors_4(mx, j, i, (dnx +i * mx->c + j)->neighbor, connection);
				add_neighbourhood_parametr_4(nx, i, j, connection);
			}
		}
	}
}


Cell* get_neighbours_8(Matrix* mx, int x, int y, int connection)
{
	Cell* neighbours = malloc(sizeof(Cell) * 3);
	int neighbourCounter = 0;

	int ny, nx;

	if (neighbours == NULL)
	{
		fprintf(stderr, "Nie udalo sie zarezerwowac pamieci na liste sasiadow");
		return NULL;
	}

	for (ny = clamp_index(y - 1, mx->r, connection); ny != clamp_index(y + 2, mx->r, connection); ny = clamp_index(++ny, mx->r, connection))
	{
		for (nx = clamp_index(x - 1, mx->c, connection); nx != clamp_index(x + 2, mx->c, connection); nx = clamp_index(++nx, mx->c, connection))
		{
			if (!(nx == x && ny == y) && mx_get_single_val(mx, ny, nx, 't') == 1)
				neighbours[neighbourCounter++] = mx_get_cell(mx, ny, nx);
		}
	}

	if (neighbourCounter != 3)
	{
		fprintf(stderr, "Komorka(%d, %d) nie posiada 3 sasiadow (%d)\n", x, y, neighbourCounter);
		return NULL;
	}

	return neighbours;
}

Cell create_cell_with_inherited_colors_8(Matrix* mx, int x, int y, int n, int connection)
{
	Cell* neighbours;

	Cell mainParent;
	Cell secondParent;

	Cell newCell =
	{
		.type = 1,
		.R = 0,
		.G = 0,
		.B = 0,
		.neighbor = n
	};

	int mainParentIndex;
	int secondParentIndex;

	int bitNumbers[3];

		neighbours = get_neighbours_8(mx, x, y, connection);

	if (neighbours == NULL)
	{
		fprintf(stderr, "Nie udalo sie znalezc sasiadow\n");
		return newCell;
	}

	// Losowanie indexow rodzicow
	mainParentIndex = rand() % 3;
	do
	{
		secondParentIndex = rand() % 3;
	} while (secondParentIndex == mainParentIndex);

	mainParent = neighbours[mainParentIndex];
	secondParent = neighbours[secondParentIndex];

	// Losowanie numerow bitow, ktore zostana dodane
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

	//printf("----\n%d %d %d \nmain %d %d %d\nsecond %d %d %d\n", newCell.R, newCell.G, newCell.B, mainParent.R, mainParent.G, mainParent.B, secondParent.R, secondParent.G, secondParent.B);
	//printf("%d %d %d\n", bitNumbers[0], bitNumbers[1], bitNumbers[2]);
	return newCell;
}

void add_neighbourhood_parametr_8(Matrix* mx, int row, int column, int connection)
{
	Cell* dmx = mx->data;
	int i;
	int maxRow = mx->r - 1;
	int maxColumn = mx->c - 1;

	if (row == 0)
	{
		if (column == 0)
		{
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + (row + 1) * mx->c + (column + 1))->neighbor++;	// gora prawo
			(dmx + row * mx->c + (column + 1))->neighbor++;			// prawo
			if (connection == 1)		// Tryb zawijania
			{
				(dmx + (row + 1) * mx->c + maxColumn)->neighbor++;	// gora lewo
				(dmx + row * mx->c + maxColumn)->neighbor++;		// lewo
				(dmx + maxRow * mx->c + maxColumn)->neighbor++;		// dol lewo
				(dmx + maxRow * mx->c + column)->neighbor++;		// dol
				(dmx + maxRow * mx->c + (column + 1))->neighbor++;	// dol prawo
			}
		}
		else if (column < maxColumn)
		{
			(dmx + (row + 1) * mx->c + (column - 1))->neighbor++;	// gora lewo
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + (row + 1) * mx->c + (column + 1))->neighbor++;	// gora prawo
			(dmx + row * mx->c + (column - 1))->neighbor++;			// lewo
			(dmx + row * mx->c + (column + 1))->neighbor++;			// prawo
			if (connection == 1)		// Tryb zawijania
			{
				(dmx + maxRow * mx->c + (column - 1))->neighbor++;	// dol lewo
				(dmx + maxRow * mx->c + column)->neighbor++;		// dol
				(dmx + maxRow * mx->c + (column + 1))->neighbor++;	// dol prawo
			}
		}
		else if (column == maxColumn)
		{
			(dmx + (row + 1) * mx->c + (column - 1))->neighbor++;	// gora lewo
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + row * mx->c + (column - 1))->neighbor++;			// lewo
			if (connection == 1)		// Tryb zawijania
			{
				(dmx + (row + 1) * mx->c)->neighbor++;				// gora prawo
				(dmx + row * mx->c)->neighbor++;					// prawo
				(dmx + maxRow * mx->c + (column - 1))->neighbor++;	// dol lewo
				(dmx + maxRow * mx->c + column)->neighbor++;		// dol
				(dmx + maxRow * mx->c)->neighbor++;					// dol prawo
			}
		}
		else
		{
			// TODO: Podany zly indeks kolumny
		}
	}
	else if (row < maxRow)
	{
		if (column == 0)
		{
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + (row + 1) * mx->c + (column + 1))->neighbor++;	// gora prawo
			(dmx + row * mx->c + (column + 1))->neighbor++;			// prawo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
			(dmx + (row - 1) * mx->c + (column + 1))->neighbor++;	// dol prawo
			if (connection == 1)		// Tryb zawijania
			{
				(dmx + (row + 1) * mx->c + maxColumn)->neighbor++;	// gora lewo
				(dmx + row * mx->c + maxColumn)->neighbor++;		// lewo
				(dmx + (row - 1) * mx->c + maxColumn)->neighbor++;	// dol lewo
			}
		}
		else if (column < maxColumn)
		{
			(dmx + (row + 1) * mx->c + (column - 1))->neighbor++;	// gora lewo
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + (row + 1) * mx->c + (column + 1))->neighbor++;	// gora prawo
			(dmx + row * mx->c + (column - 1))->neighbor++;			// lewo
			(dmx + row * mx->c + (column + 1))->neighbor++;			// prawo
			(dmx + (row - 1) * mx->c + (column - 1))->neighbor++;	// dol lewo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
			(dmx + (row - 1) * mx->c + (column + 1))->neighbor++;	// dol prawo
		}
		else if (column == maxColumn)
		{
			(dmx + (row + 1) * mx->c + (column - 1))->neighbor++;	// gora lewo
			(dmx + (row + 1) * mx->c + column)->neighbor++;			// gora
			(dmx + row * mx->c + (column - 1))->neighbor++;			// lewo
			(dmx + (row - 1) * mx->c + (column - 1))->neighbor++;	// dol lewo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
			if (connection == 1)		// Tryb zawijania
			{
				(dmx + (row + 1) * mx->c)->neighbor++;				// gora prawo
				(dmx + row * mx->c)->neighbor++;					// prawo
				(dmx + (row - 1) * mx->c)->neighbor++;				// dol prawo
			}
		}
		else
		{
			// TODO: Podany zly indeks kolumny
		}
	}
	else if (row == maxRow)
	{
		if (column == 0)
		{
			(dmx + row * mx->c + (column + 1))->neighbor++;			// prawo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
			(dmx + (row - 1) * mx->c + (column + 1))->neighbor++;	// dol prawo
			if (connection == 1)		// Tryb zawijania
			{
				(dmx + maxColumn)->neighbor++;						// gora lewo
				(dmx + column)->neighbor++;							// gora
				(dmx + (column + 1))->neighbor++;					// gora prawo
				(dmx + row * mx->c + maxColumn)->neighbor++;		// lewo
				(dmx + (row - 1) * mx->c + maxColumn)->neighbor++;	// dol lewo
			}
		}
		else if (column < maxColumn)
		{
			(dmx + row * mx->c + (column - 1))->neighbor++;			// lewo
			(dmx + row * mx->c + (column + 1))->neighbor++;			// prawo
			(dmx + (row - 1) * mx->c + (column - 1))->neighbor++;	// dol lewo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
			(dmx + (row - 1) * mx->c + (column + 1))->neighbor++;	// dol prawo
			if (connection == 1)		// Tryb zawijania
			{
				(dmx + (column - 1))->neighbor++;;					// gora lewo
				(dmx + column)->neighbor++;							// gora
				(dmx + (column + 1))->neighbor++;;					// gora prawo
			}
		}
		else if (column == maxColumn)
		{
			(dmx + row * mx->c + (column - 1))->neighbor++;			// lewo
			(dmx + (row - 1) * mx->c + (column - 1))->neighbor++;	// dol lewo
			(dmx + (row - 1) * mx->c + column)->neighbor++;			// dol
			if (connection == 1)		// Tryb zawijania
			{
				(dmx + (column - 1))->neighbor++;					// gora lewo
				(dmx + column)->neighbor++;							// gora
				(dmx + 0)->neighbor++;								// gora prawo
				(dmx + row * mx->c)->neighbor++;					// prawo
				(dmx + (row - 1) * mx->c)->neighbor++;				// dol prawo
			}
		}
		else
		{
			// TODO: Podany zly indeks kolumny
		}
	}
	else
	{
		// TODO: Podany zly indeks rzedu
	}
}

void count_neighbours_8(Matrix* mx, int connection)
{
	for (int i = 0; i < mx->r; i++)
		for (int j = 0; j < mx->c; j++)
		{
			if (mx_get_single_val(mx, i, j, 't') == 1)
				add_neighbourhood_parametr_8(mx, i, j, connection);
		}
}

void make_a_cycle_rewrite_struct_8(Matrix* mx, Matrix* nx, int maxRow, int maxColumn, int connection)
{
	Cell* dmx = mx->data;
	Cell* dnx = nx->data;
	int i;
	int j;

	for (i = 0; i <= maxRow; i++)
	{
		for (j = 0; j <= maxColumn; j++)
		{
			if ((dmx + i * mx->c + j)->type == 2)		// sciana
			{
				(dnx + i * mx->c + j)->type = 2;
			}
			else if ((dmx + i * mx->c + j)->type == 1)		// zywa
			{
				if ((dmx + i * mx->c + j)->neighbor == 2 || (dmx + i * mx->c + j)->neighbor == 3)
				{		//przepisuje punkt zywy do nowej strukturze
					(dnx + i * nx->c + j)->type = 1;
					add_neighbourhood_parametr_8(nx, i, j, connection);		//dodaje parametr sasiad
					(dnx + i * mx->c + j)->R = (dmx + i * mx->c + j)->R;
					(dnx + i * mx->c + j)->G = (dmx + i * mx->c + j)->G;
					(dnx + i * mx->c + j)->B = (dmx + i * mx->c + j)->B;
				}
			}
			else if ((dmx + i * mx->c + j)->type == 0 && (dmx + i * mx->c + j)->neighbor == 3)
			{
				dnx[i * mx->c + j] = create_cell_with_inherited_colors_8(mx, j, i, (dnx +i * mx->c + j)->neighbor, connection);
				add_neighbourhood_parametr_8(nx, i, j, connection);
			}
		}
	}
}

