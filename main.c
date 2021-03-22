#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cell.h"
#include "cycle.h"
#include "image_creator.h"

//#if defined (_WIN64)
//#include "getopt.h"
//#elif defined (__unix__) || defined(__linux__)
//#include <unistd.h>
//#endif

// TODO: /\ Cos nie do konca dziala w VS

#include "getopt.h"

#define DEFAULT_NUMBER_OF_CYCLES 1
#define DEFAULT_OUTPUT_SCALE 1;

//void mx_write_types(Matrix* mx)
//{
//	for (int i = mx->r-1; i >=0; i--)
//	{
//		for (int j = 0; j < mx->c; j++)
//		{
//			printf("%d ", mx_get_single_val(mx, i, j, 't'));
//		}
//		printf("\n");
//	}
//	printf("\n");
//}

//void mx_write_neighbours(Matrix* mx)
//{
//	for (int i = mx->r - 1; i >= 0; i--)
//	{
//		for (int j = 0; j < mx->c; j++)
//		{
//			printf("%d ", mx_get_single_val(mx, i, j, 'n'));
//		}
//		printf("\n");
//	}
//	printf("\n");
//}

int main(int argc, char* argv[])
{
	FILE* inputFile = NULL;
	FILE* bmpOutputFile = NULL;
	FILE* pngOutputFile = NULL;

	int numberOfCycles = DEFAULT_NUMBER_OF_CYCLES;
	int scale = DEFAULT_OUTPUT_SCALE;

	Matrix* board = NULL;
	Matrix* nx = NULL;

	char c;

	srand(time(NULL));

	while ((c = getopt(argc, argv, "f:p:b:n:s:")) != -1)
	{
		switch (c)
		{
		case 'f':	// Plik z danymi
			inputFile = fopen(optarg, "r");

			if (inputFile == NULL)
				fprintf(stderr, "Nie mozna otworzyc pliku do odczytu: %s\n", optarg);
			break;

		case 'p':	// Plik do zapisu (.png)
			pngOutputFile = fopen(optarg, "wb");

			if (pngOutputFile == NULL)
				fprintf(stderr, "Nie mozna stworzyc pliku (.png) do zapisu obrazu: %s\n", optarg);
			break;

		case 'b':	// Plik do zapisu (.bmp)
			bmpOutputFile = fopen(optarg, "wb");

			if (bmpOutputFile == NULL)
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

	if (inputFile == NULL)		// Ustawianie "strumienia wejsciowego" na stdin
	{
		printf("Podaj dane wejsciowe\n");
		inputFile = stdin;
	}

	// Tworzenie macierzy
	board = mx_read_from_file(inputFile);		// Czytanie z pliku
	nx = mx_alloc(board->r, board->c);
	//printf("typy\n");
	//mx_write_types(board);

	
	if (board->nei == 1)		// Tryb 4 sasiadow
	{
		count_neighbours_4(board);					// Liczenie sasiadow dla 0 cyklu

		//save_as_bitmap(fopen("e:/ZZ_GOL/bbb.bmp", "wb"), board, scale);
		//printf("sasiedztwo\n");
		//mx_write_neighbours(board);

		for (int i = 0; i < numberOfCycles; i++)
		{
			make_a_cycle_rewrite_struct_4(board, nx, board->c - 1, board->r - 1, board->nei);
			mx_cpy(nx, board);
			//printf("%d  typy\n", i);
			//mx_write_types(board);
			//printf("sasiedztwo\n");
			//mx_write_neighbours(board);
		}
	}
	else						// Tryb 8 sasiadow
	{
		count_neighbours_8(board);					// Liczenie sasiadow dla 0 cyklu

		//save_as_bitmap(fopen("e:/ZZ_GOL/bbb.bmp", "wb"), board, scale);
		//printf("sasiedztwo\n");
		//mx_write_neighbours(board);

		for (int i = 0; i < numberOfCycles; i++)
		{
			make_a_cycle_rewrite_struct_8(board, nx, board->c - 1, board->r - 1, board->nei);
			mx_cpy(nx, board);
			//printf("%d-------------- typy\n", i);
			//mx_write_types(board);
			//printf("sasiedztwo\n");
			//mx_write_neighbours(board);	
		}
	}

	if (bmpOutputFile != NULL)	// Zapis do pliku .bmp
		save_as_bitmap(bmpOutputFile, board, scale);

	if (pngOutputFile != NULL)	// Zapis do pliku .png
	{
		/*process_png_file(board, scale);
		write_png_file(pngOutputFile);*/
	}


	// HACK: Tylko potrzebne mi do odpalania programu w VS
	getchar();

	return 0;
}