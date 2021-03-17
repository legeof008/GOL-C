#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "cycle.h"
#include "getopt.h"
#include "image_creator.h"

// TODO: Potem usune to i zastapie "null" na "NULL"
#define null NULL

#define DEFAULT_NUMBER_OF_CYCLES 1
#define DEFAULT_OUTPUT_SCALE 1;

// TODO: Tych 3 funkcji nie powinno byc w tym pliku
void count_neighbours(Matrix *mx)
{
	for (int i = 0; i < mx->r; i++)
		for (int j = 0; j < mx->c; j++)
			add_neighbourhood_parametr(mx, mx->c - 1, mx->r - 1, mx->nei, i, j, mx->fol);	// TODO: nwm czy w ogole dobrze uzywam tej funkcji
}

void mx_free(Matrix* mx)
{
	free(mx->data);
	free(mx);
	mx = NULL;
}

void mx_clear(Matrix* mx)
{
	Cell empty =
	{
		.type = 1,
		.R = 0,
		.G = 0,
		.B = 0,
		.neighbor = 0
	};

	for (int i = 0; i < mx->r; i++)
		for (int j = 0; j < mx->c; j++)
			mx->data[j + i * mx->c] = empty;
}

int main(int argc, char* argv[])
{
	FILE* inputFile = null;
	FILE* bmpOutputFile = null;
	FILE* pngOutputFile = null;

	int numberOfCycles = DEFAULT_NUMBER_OF_CYCLES;
	int scale = DEFAULT_OUTPUT_SCALE;

	Matrix* board = null;
	Matrix* nx = null;

	char c;

	// TODO: s: jest tylko na razie do ustalania skali. moze w przyszlosci skala bedzie argumentem wywolania
	while ((c = getopt(argc, argv, "f:p:b:n:s:")) != -1)
	{
		switch (c)
		{
		case 'f':	// Plik z danymi
			inputFile = fopen(optarg, "r");

			if (inputFile == null)
				fprintf(stderr, "Nie mozna otworzyc pliku do odczytu: %s\n", optarg);
			break;

		case 'p':	// Plik do zapisu (.png)
			pngOutputFile = fopen(optarg, "wb");

			if (pngOutputFile == null)
				fprintf(stderr, "Nie mozna stworzyc pliku (.png) do zapisu obrazu: %s\n", optarg);
			break;

		case 'b':	// Plik do zapisu (.bmp)
			bmpOutputFile = fopen(optarg, "wb");

			if (bmpOutputFile == null)
				fprintf(stderr, "Nie mozna stworzyc pliku (.bmp) do zapisu obrazu: %s\n", optarg);
			break;

		case 'n':	// Liczba cykli
			numberOfCycles = atoi(optarg);
			break;

		case 's':
			scale = atoi(optarg);
			break;

		case '?':
			fprintf(stderr, "Podano nieznany argument\n");
			break;

		default:
			break;
		}
	}

	if (inputFile == null)		// Ustawianie "strumienia wejsciowego" na stdin
	{
		printf("Podaj dane wejsciowe\n");
		inputFile = stdin;
	}

	board = mx_read_from_file(inputFile);		// Czytanie z pliku

	nx = mx_alloc(board->r, board->c);

	count_neighbours(board);					// Liczenie sasiadow dla 0 cyklu

	for (int i = 0; i < numberOfCycles; i++)
	{
		make_a_cycle_rewrite_struct(board, nx, board->c - 1, board->r - 1, board->nei);
		mx_cpy(nx, board);
		mx_clear(nx);
	}


	if (bmpOutputFile != null)	// Zapis do pliku .bmp
		save_as_bitmap(bmpOutputFile, board, scale);

	if (pngOutputFile != null)	// Zapis do pliku .png
		;// TODO: Zapis do .png

	mx_free(board);
	mx_free(nx);



	// HACK: Tylko potrzebne mi do odpalania programu w VS
	getchar();

	return 0;
}