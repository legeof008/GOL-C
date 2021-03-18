#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "cell.h"


#define CH (char) // nie chce mi sie pisac
#define COL "\x1b[38;2;"

void mx_init(Matrix *mx) // inicjalizuje macierz, na białą planszę martwych komórek,
{
	int col = mx->c;
	Cell *dmx = (Cell *)malloc(mx->r * col * sizeof(Cell));
	for (int i = 0; i < mx->r; i++)
	{
		for (int j = 0; j < mx->c; j++)
		{
			(dmx + i * col + j)->type = CH 0x00;
			(dmx + i * col + j)->R = CH 0xFF;
			(dmx + i * col + j)->G = CH 0xFF;
			(dmx + i * col + j)->B = CH 0xFF;
			(dmx + i * col + j)->neighbor = CH 0x00;
		}
	}
	mx->data = dmx;
}

int mx_put_val(Matrix *mx, int r, int c, int num, ...) /* wieloargumentowa funkcja, NIE dawać więcej niż 3 argumenty po num*/
{
	Cell *dmx = mx->data;
	va_list ap;
	va_start(ap, num);
	if (num == 1) // num = 1 zmiana typu komórki
	{
		(dmx + r * mx->c + c)->type = (char)va_arg(ap, int);
		return 1;
	}
	if (num == 2) // num = 2 zmiana sąsiedztwa
	{
		(dmx + r * mx->c + c)->neighbor = (char)va_arg(ap, int);
		return 2;
	}
	if (num == 3) // num = 3 zmiana kolejno R G B
	{
		(dmx + r * mx->c + c)->R = (char)va_arg(ap, int);
		(dmx + r * mx->c + c)->G = (char)va_arg(ap, int);
		(dmx + r * mx->c + c)->B = (char)va_arg(ap, int);
		mx->data = dmx;
		return 2;
	}
	else
		return 0;
}

Matrix *mx_alloc(int r, int c) // alokuje pamięć na Matrix
{
	Matrix *A = (Matrix *)malloc(sizeof(Matrix));
	A->c = c;
	A->r = r;
	mx_init(A);
	return A;
}

Matrix *mx_read_from_file(char *filename) // Czyta z pliku
{
	int x, y, type, R, G, B;
	FILE *f = fopen(filename, "r");
	Matrix *A = (Matrix *)malloc(sizeof(Matrix));
	fscanf(f, "%d %d %c %c", &(A->r), &(A->c), &(A->nei), &(A->fol));
	mx_init(A);
	while (fscanf(f, "%d %d %d %d %d %d", &x, &y, &type, &R, &G, &B) == 6)
	{
		mx_put_val(A, x, y, 1, CH type);
		mx_put_val(A, x, y, 3, CH R, CH G, CH B);
	}
	return A;
}

char *mx_get_col(Cell *dna)
{

	char *colbash = malloc(23 * sizeof(char));
	strcpy(colbash, COL);
	char *result = malloc(3 * sizeof(char));
	sprintf(result, "%d", ((dna)->G + 256));
	strcat(colbash, result);
	strcat(colbash, ";");
	sprintf(result, "%d", ((dna)->G + 256));
	strcat(colbash, result);
	strcat(colbash, ";");
	sprintf(result, "%d", ((dna)->B + 256));
	strcat(colbash, result);
	strcat(colbash, "m");

	return colbash;
}

void mx_print(Matrix *mx) // wypisuje jak macierz poglądowo
{
	Cell *dmx = mx->data;
	for (int i = 0; i < mx->r; i++)
	{
		for (int j = 0; j < mx->c; j++)
		{
			printf("%s%d ", mx_get_col(dmx + i * mx->c + j), (dmx + i * mx->c + j)->type);
		}
		printf("\n");
	}
}

char mx_get_single_val(Matrix *mx, int r, int c, char type) // wydobycie pojedynczej cechy komórki, t- type, n - sąsiad
{
	Cell *dmx = mx->data;
	char ret;
	switch (type)
	{
	case 't': // dla typu
		ret = (dmx + r * mx->c + c)->type;
		printf("%d\n", (int)ret);
		return ret;
		break;

	case 'n': // sąsiad
		ret = (dmx + r * mx->c + c)->neighbor;
		return ret;
		break;

	case 'r':	// czerwony kolor
		return (dmx + r * mx->c + c)->R;

	case 'g':	// zielony kolor
		return (dmx + r * mx->c + c)->G;

	case 'b':	// niebieski kolor
		return (dmx + r * mx->c + c)->B;

	default: // dla dobrej miary do debugowania
		return 'x';
		break;
	}
}

//char mx_print_rgb(Matrix* mx )
/*
int main(int argc, char **argv)
{
	Matrix *mxt = mx_read_from_file(argv[0]);
	mx_print(mxt);

	return 0;
}
*/
