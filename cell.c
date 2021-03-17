#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "cell.h"

#define CH (char) // nie chce mi sie pisac

void mx_init(Matrix * mx) // inicjalizuje macierz, na białą planszę martwych komórek,
{
	int col = mx->c;
	Cell* dmx = (Cell*)malloc(mx->r * col * sizeof(Cell));
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

int mx_put_val(Matrix* mx, int r, int c, int num, ...) /* wieloargumentowa funkcja, NIE dawać więcej niż 3 argumenty po num*/
{
	Cell* dmx = mx->data;
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

Matrix* mx_alloc(int r, int c) // alokuje pamięć na Matrix
{
	Matrix* A = (Matrix*)malloc(sizeof(Matrix));
	A->c = c;
	A->r = r;
	mx_init(A);
	return A;
}

Matrix* mx_read_from_file(FILE *f) // Czyta z pliku
{
	int x, y, type, R, G, B;

	if (f == NULL)
	{
		fprintf(stderr, "Error opening file: %s\n", filename);
		return NULL;
	}
	Matrix *A = (Matrix *)malloc(sizeof(Matrix));
	fscanf(f, "%d %d %c %c", &(A->r), &(A->c), &(A->nei), &(A->fol));
	mx_init(A);
	while (fscanf(f, "%d %d %d %d %d %d", &x, &y, &type, &R, &G, &B) == 6)
	{
		mx_put_val(A, y, x, 1, CH type);
		mx_put_val(A, y, x, 3, CH R, CH G, CH B);
	}
	return A;
}

void mx_print(Matrix *mx) // wypisuje jak macierz poglądowo
{
	Cell* dmx = mx->data;
	for (int i = 0; i < mx->r; i++)
	{
		for (int j = 0; j < mx->c; j++)
		{
			printf("%d ", (dmx + i * mx->c + j)->type);
		}
		printf("\n");
	}
}

Cell *mx_get_cell(Matrix *mx, int r, int c)
{
	Cell *dmx = mx->data;
	return (dmx + r * mx->c + c);
}
char mx_get_single_val(Matrix *mx, int r, int c, char type) // wydobycie pojedynczej cechy komórki, t- type, n - sąsiad
{
	Cell* dmx = mx->data;
	char ret;
	switch (type)
	{
	case 't': // dla typu

		return (dmx + r * mx->c + c)->type;
		break;

	case 'n': // sąsiad
		return (dmx + r * mx->c + c)->neighbor;
		break;

	case 'r': // czerwony kolor
		return (dmx + r * mx->c + c)->R;
		break;

	case 'g': // zielony kolor
		return (dmx + r * mx->c + c)->G;
		break;

	case 'b': // niebieski kolor
		return (dmx + r * mx->c + c)->B;
		break;

	default: // dla dobrej miary do debugowania
		return 'x';
		break;
	}
}
void mx_cpy(Matrix *src, Matrix *dest) //Kopiowanie macierzy z zerowaniem src
{
	int col = src->c;
	Cell *srcd = src->data;
	Cell *destd = dest->data;
	for (int i = 0; i < src->c; i++)
	{
		for (int j = 0; j < src->c; j++)
		{
			(destd + i * col + j)->R = (srcd + i * col + j)->R;
			(destd + i * col + j)->G = (srcd + i * col + j)->G;
			(destd + i * col + j)->B = (srcd + i * col + j)->B;
			(destd + i * col + j)->type = (srcd + i * col + j)->type;
			(destd + i * col + j)->neighbor = (srcd + i * col + j)->neighbor;
			(srcd + i * col + j)->type = CH 0x00;
			(srcd + i * col + j)->R = CH 0xFF;
			(srcd + i * col + j)->G = CH 0xFF;
			(srcd + i * col + j)->B = CH 0xFF;
			(srcd + i * col + j)->neighbor = CH 0x00;
		}
	}
}
/*int main(int argc, char **argv)
{
	FILE *in = argc > 1 ? fopen(argv[0], "r") : stdin;
	Matrix *mxt = mx_read_from_file(in);
	mx_print(mxt);
	Cell *dmc = mx_get_cell(mxt, 0, 0);
	printf("%d\n", dmc->type);

	return 0;
}*/
