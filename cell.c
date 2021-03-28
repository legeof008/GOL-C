#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "cell.h"

#define CH (char) // nie chce mi sie pisac
#define MAX_SIZE 100 // TODO: do ustalenia

int mx_init(Matrix * mx) // inicjalizuje macierz, na białą planszę martwych komórek,
{
	int col = mx->c;
	Cell* dmx = (Cell*)malloc(mx->r * col * sizeof(Cell));
	if(dmx == NULL)
	{
		fprintf(stderr, "[mx_init] ERROR, nie mozna zaalokowac pamieci na plansze\n");
		return -1;
	}
	for (int i = 0; i < mx->r; i++)
	{
		for (int j = 0; j < mx->c; j++)
		{
			(dmx + i * col + j)->type = CH 0x00;
			(dmx + i * col + j)->R = CH 0x00;
			(dmx + i * col + j)->G = CH 0x00;
			(dmx + i * col + j)->B = CH 0x00;
			(dmx + i * col + j)->neighbor = CH 0x00;
		}
	}
	mx->data = dmx;

	return 0;
}
void mx_free(Matrix* mx)
{
	free(mx->data);
	free(mx);
}

int mx_put_val(Matrix* mx, int r, int c, int num, ...) /* wieloargumentowa funkcja, NIE dawać więcej niż 3 argumenty po num*/
{
	Cell* dmx = mx->data;
	if(dmx == NULL)
	{
		fprintf(stderr, "[mx_put_val] ERROR, nie udalo sie zaalokowac pamieci na komorke\n");
		return -1;
	}
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
	int d = mx_init(A);
	return A;
	
}

Matrix* mx_read_from_file(FILE *f) // Wczytuje dane z pliku lub ze standardowego wejscia
{
	int x, y, type, R, G, B;

	Matrix *A = (Matrix *)malloc(sizeof(Matrix));
	if (A == NULL)
	{
		fprintf(stderr, "[mx_red_from_file]: ERROR, nie mozna zaalokowac pamieci na plansze\n");
		return NULL;
	}

	if(fscanf(f, "%d", &(A->r)) != 1)
	{
		fprintf(stderr, "[mx_red_from_file]: ERROR, nie podano wysokosci planszy\n");
		return NULL;
	}
	if (A->r <= 0 || A->r > MAX_SIZE)
	{
		fprintf(stderr, "[mx_red_from_file]: ERROR, Podano nieprawidlowa wysokosc planszy: %d\n"
		"Wysokosc musi nalezec do przedzialu (0, %d>\n", A->r, MAX_SIZE);
		return NULL;
	}

	if(fscanf(f, "%d", &(A->c)) != 1)
	{
		fprintf(stderr, "[mx_red_from_file]: ERROR, nie podano szerokosci planszy\n");
		return NULL;
	}
	if (A->c <= 0 || A->c > MAX_SIZE)
	{
		fprintf(stderr, "[mx_red_from_file]: ERROR, Podano nieprawidlowa szerokosc planszy: %d\n"
		"Szerokosc musi nalezec do przedzialu (0, %d>\n", A->c, MAX_SIZE);
		return NULL;
	}

	if (mx_init(A) != 0)
		return NULL;

	while(1)
	{
		if (fscanf(f, "%d", &x) != 1)
			break;
		if ( x >= A->c || x < 0)
		{
			fprintf(stderr, "[mx_red_from_file]: ERROR, Podano nieprawidlowa wspolrzedna x: %d\n"
			"Wspolrzedna x musi nalezec do przedzialu (0, %d>\n", x, A->c-1);
			return NULL;
		}

		if (fscanf(f, "%d", &y) != 1)
		{
			fprintf(stderr, "[mx_red_from_file]: ERROR, Po podaniu wspolrzednej x: %d nie podano wspolrzednej y\n", x);
			return NULL;
		}
		if ( y >= A->r || y < 0)
		{
			fprintf(stderr, "[mx_red_from_file]: ERROR, Podano nieprawidlowa wspolrzedna y: %d\n"
			"Wspolrzedna y musi nalezec do przedzialu (0, %d>\n", y, A->r-1);
			return NULL;
		}

		if (fscanf(f, "%d", &type) != 1)
		{
			fprintf(stderr, "[mx_red_from_file]: ERROR, Po podaniu wspolrzednych nie podano typu\n"
			"Typ 0 - kom. martwa, 1 - kom. zywa, 2 - sciana\n");
			return NULL;
		}
		if (type < 0 || type > 2)
		{
			fprintf(stderr, "[mx_red_from_file]: ERROR, Podano nieprawidlowy typ: %d\n"
			"Typ 0 - kom. martwa, 1 - kom. zywa, 2 - sciana\n", type);
			return NULL;
		}

		if (fscanf(f, "%d", &R) != 1)
		{
			fprintf(stderr, "[mx_red_from_file]: ERROR, Po podaniu wspolrzednych i typu nie podano wartosci parametru R\n");
			return NULL;
		}
		if( R < 0 || R > 255)
		{
			fprintf(stderr, "[mx_red_from_file]: ERROR, Nieprawidlowa wartosc parametru R: %d\n"
			"Parametr R musi nalezec do przedzialu <0,255>\n", R);
			return NULL;
		}

		if (fscanf(f, "%d", &G) != 1)
		{
			fprintf(stderr, "[mx_red_from_file]: ERROR, Po podaniu wspolrzednych i typu nie podano wartosci parametru G\n");
			return NULL;
		}
		if( G < 0 || G > 255)
		{
			fprintf(stderr, "[mx_red_from_file]: ERROR, Nieprawidlowa wartosc parametru G: %d\n"
			"Parametr G musi nalezec do przedzialu <0,255>\n", G);
			return NULL;
		}

		if (fscanf(f, "%d", &B) != 1)
		{
			fprintf(stderr, "[mx_red_from_file]: ERROR, Po podaniu wspolrzednych i typu nie podano wartosci parametru B\n");
			return NULL;
		}
		if( B < 0 || B > 255)
		{
			fprintf(stderr, "[mx_red_from_file]: ERROR, Nieprawidlowa wartosc parametru B: %d\n"
			"Parametr B musi nalezec do przedzialu <0,255>\n", B);
			return NULL;
		}

		if (mx_put_val(A, y, x, 1, CH type) < 0)
			return NULL;
		if (mx_put_val(A, y, x, 3, CH R, CH G, CH B) < 0)
			return NULL;
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

Cell mx_get_cell(Matrix *mx, int r, int c)
{
	Cell *dmx = mx->data;
	return (dmx[r * mx->c + c]);
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

	for (int i = 0; i < src->r; i++)
	{
		for (int j = 0; j < src->c; j++)
		{
			/*
			((dest->data) + i* col+j)->R = ((src->data)+i*col+j)->R;
			((dest->data) + i* col+j)->R = ((src->data)+i*col+j)->G;
			((dest->data) + i* col+j)->R = ((src->data)+i*col+j)->B;
			((dest->data) + i* col+j)->type = ((src->data)+i*col+j)->type;
			((dest->data) + i* col+j)->neighbor = ((src->data)+i*col+j)->neighbor;
			((src->data)+i*col+j)->type = CH 0x00;
			((src->data)+i*col+j)->R = CH 0x00;
			((src->data)+i*col+j)->G = CH 0x00;
			((src->data)+i*col+j)->B = CH 0x00;
			((src->data)+i*col+j)->neighbor = CH 0x00;*/

			(destd + i * col + j)->R = (srcd + i * col + j)->R;
			(destd + i * col + j)->G = (srcd + i * col + j)->G;
			(destd + i * col + j)->B = (srcd + i * col + j)->B;
			(destd + i * col + j)->type = (srcd + i * col + j)->type;
			(destd + i * col + j)->neighbor = (srcd + i * col + j)->neighbor;
			(srcd + i * col + j)->type = CH 0x00;
			(srcd + i * col + j)->R = CH 0x00;
			(srcd + i * col + j)->G = CH 0x00;
			(srcd + i * col + j)->B = CH 0x00;
			(srcd + i * col + j)->neighbor = CH 0x00;
		}
	}
	src->data = srcd;
	dest->data = destd;
}
