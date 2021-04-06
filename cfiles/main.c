#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "headers/cell.h"
#include "headers/cycle.h"
#include "headers/image_creator.h"

#include <unistd.h>

#define DEFAULT_NUMBER_OF_CYCLES 10
#define DEFAULT_NEIGBOUR_TYPE 8
#define DEFAULT_OUTPUT_SCALE 1

int main(int argc, char* argv[])
{
	FILE* inputFile = NULL;
	FILE* bmpOutputFile = NULL;
	FILE* pngOutputFile = NULL;

	int numberOfCycles = DEFAULT_NUMBER_OF_CYCLES;
	int neighbourType = DEFAULT_NEIGBOUR_TYPE;
	int fold = 0;
	int scale = DEFAULT_OUTPUT_SCALE;

	Matrix* board = NULL;
	Matrix* nx = NULL;

	char c;

	srand(time(NULL));

	while ((c = getopt(argc, argv, "f:p:b:c:s:nz")) != -1)
	{
		switch (c)
		{
		case 'f':	// Plik z danymi
			inputFile = fopen(optarg, "r");

			if (inputFile == NULL)
				fprintf(stderr, "Nie mozna otworzyc pliku do odczytu: -f %s\n", optarg);
			break;

		case 'p':	// Plik do zapisu (.png)
			pngOutputFile = fopen(optarg, "wb");

			if (pngOutputFile == NULL)
				fprintf(stderr, "Nie mozna stworzyc pliku (.png) do zapisu obrazu: -p %s\n", optarg);
			break;

		case 'b':	// Plik do zapisu (.bmp)
			bmpOutputFile = fopen(optarg, "wb");

			if (bmpOutputFile == NULL)
				fprintf(stderr, "Nie mozna stworzyc pliku (.bmp) do zapisu obrazu: -b %s\n", optarg);
			break;

		case 'c':	// Liczba cykli
			numberOfCycles = atoi(optarg);

			if (numberOfCycles < 0)
			{
				fprintf(stderr, "Podano nieprawidlowa liczbe cykli -c %d\n", numberOfCycles);
				numberOfCycles = DEFAULT_NUMBER_OF_CYCLES;
			}
			break;

		case 's':	// Skala zapisanego obrazu
			scale = atoi(optarg);
			if (scale < 1 || scale > 10)
			{
				fprintf(stderr, "Podano nieprawidlowa skale -s %d\nDozwolony przedzial <1, 10>\n", numberOfCycles);
				scale = DEFAULT_OUTPUT_SCALE;
			}
			break;

		case 'n':
			neighbourType = 4;
			break;

		case 'z':
			fold = 1;
			break;

		case '?':
			fprintf(stderr, "Podano nieznany argument\n");
			break;

		default:
			break;
		}
	}

	// Sprawdzanie czy plik wyjsciowy zostal podany
	if (bmpOutputFile == NULL && pngOutputFile == NULL)
	{
		fprintf(stderr, "Nie podano zadnego pliku wyjsciowego\n");
		exit(EXIT_FAILURE);
	}

	// Ustawianie "strumienia wejsciowego" na stdin
	if (inputFile == NULL)		
	{
		printf("Podaj dane wejsciowe\n");
		inputFile = stdin;
	}

	// Tworzenie macierzy
	board = mx_read_from_file(inputFile);		// Czytanie z pliku
	if (board == NULL)
	{
		fprintf(stderr, "Nie mozna stworzyc macierzy!\n");
		exit(EXIT_FAILURE);
	}

	nx = mx_alloc(board->r, board->c);
	if (nx == NULL)
	{
		fprintf(stderr, "Nie mozna stworzyc macierzy!\n");
		exit(EXIT_FAILURE);
	}

	if (neighbourType == 4)		// Tryb 4 sasiadow
	{
		count_neighbours_4(board, fold);					// Liczenie sasiadow dla 0 cyklu

		for (int i = 0; i < numberOfCycles; i++)
		{
			make_a_cycle_rewrite_struct_4(board, nx, board->c - 1, board->r - 1, fold);
			mx_cpy(nx, board);
		}
	}
	else						// Tryb 8 sasiadow
	{
		count_neighbours_8(board, fold);					// Liczenie sasiadow dla 0 cyklu

		for (int i = 0; i < numberOfCycles; i++)
		{
			make_a_cycle_rewrite_struct_8(board, nx, board->c - 1, board->r - 1, fold);
			mx_cpy(nx, board);
		}
	}
	if (bmpOutputFile != NULL)	// Zapis do pliku .bmp
	{
		save_as_bitmap(bmpOutputFile, board, scale);
	}

	if (pngOutputFile != NULL)	// Zapis do pliku .png
	{
		process_png_file(board, scale);
		write_png_file(pngOutputFile);
	}

	// Zwalnianie macierzy
	mx_free(board);
	mx_free(nx);

	exit(EXIT_SUCCESS);
}
