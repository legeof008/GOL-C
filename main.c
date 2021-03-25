#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cell.h"
#include "cycle.h"
#include "image_creator.h"

#if defined(_MSC_VER)
#include "getopt.h"
#elif defined(__GNUC__)
#include <unistd.h>
#endif

#define DEFAULT_NUMBER_OF_CYCLES 1
#define DEFAULT_NEIGBOUR_TYPE 8
#define DEFAULT_OUTPUT_SCALE 1

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
//
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

		case 'c':	// Liczba cykli
			numberOfCycles = atoi(optarg);
			break;

		case 's':	// Skala zapisanego obrazu
			scale = atoi(optarg);
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

	if (inputFile == NULL)		// Ustawianie "strumienia wejsciowego" na stdin
	{
		printf("Podaj dane wejsciowe\n");
		inputFile = stdin;
	}

	// Tworzenie macierzy
	board = mx_read_from_file(inputFile);		// Czytanie z pliku
	if(board == NULL)
		return -1;

	nx = mx_alloc(board->r, board->c);
	//printf("typy\n");
	//mx_write_types(board);

	/*char* pat[] = {
		"e:/ZZ_GOL/b1.bmp",
		"e:/ZZ_GOL/b2.bmp",
		"e:/ZZ_GOL/b3.bmp",
		"e:/ZZ_GOL/b4.bmp",
		"e:/ZZ_GOL/b5.bmp",
		"e:/ZZ_GOL/b6.bmp",
		"e:/ZZ_GOL/b7.bmp",
		"e:/ZZ_GOL/b8.bmp",
		"e:/ZZ_GOL/b9.bmp",
		"e:/ZZ_GOL/b10.bmp",
		"e:/ZZ_GOL/b11.bmp",
		"e:/ZZ_GOL/b12.bmp",
		"e:/ZZ_GOL/b13.bmp",
		"e:/ZZ_GOL/b14.bmp",
		"e:/ZZ_GOL/b15.bmp",
		"e:/ZZ_GOL/b16.bmp",
		"e:/ZZ_GOL/b17.bmp",
		"e:/ZZ_GOL/b18.bmp",
		"e:/ZZ_GOL/b19.bmp",
		"e:/ZZ_GOL/b20.bmp",
		"e:/ZZ_GOL/b21.bmp",
		"e:/ZZ_GOL/b22.bmp",
		"e:/ZZ_GOL/b23.bmp",
		"e:/ZZ_GOL/b24.bmp",
		"e:/ZZ_GOL/b25.bmp",
		"e:/ZZ_GOL/b26.bmp",
		"e:/ZZ_GOL/b27.bmp",
		"e:/ZZ_GOL/b28.bmp",
		"e:/ZZ_GOL/b29.bmp",
		"e:/ZZ_GOL/b30.bmp",
		"e:/ZZ_GOL/b31.bmp",
		"e:/ZZ_GOL/b32.bmp",
		"e:/ZZ_GOL/b33.bmp",
		"e:/ZZ_GOL/b34.bmp",
		"e:/ZZ_GOL/b35.bmp",
		"e:/ZZ_GOL/b36.bmp",
		"e:/ZZ_GOL/b37.bmp",
		"e:/ZZ_GOL/b38.bmp",
		"e:/ZZ_GOL/b39.bmp",
		"e:/ZZ_GOL/b40.bmp",
		"e:/ZZ_GOL/b41.bmp",
		"e:/ZZ_GOL/b42.bmp",
		"e:/ZZ_GOL/b43.bmp",
		"e:/ZZ_GOL/b44.bmp",
		"e:/ZZ_GOL/b45.bmp",
		"e:/ZZ_GOL/b46.bmp",
		"e:/ZZ_GOL/b47.bmp",
		"e:/ZZ_GOL/b48.bmp",
		"e:/ZZ_GOL/b49.bmp",
	};*/

	if (neighbourType == 4)		// Tryb 4 sasiadow
	{
		count_neighbours_4(board, fold);					// Liczenie sasiadow dla 0 cyklu

		//save_as_bitmap(fopen("e:/ZZ_GOL/bbb.bmp", "wb"), board, scale);
		//printf("sasiedztwo\n");
		//mx_write_neighbours(board);

		for (int i = 0; i < numberOfCycles; i++)
		{
			make_a_cycle_rewrite_struct_4(board, nx, board->c - 1, board->r - 1, fold);
			mx_cpy(nx, board);
			//printf("%d  typy\n", i);
			//mx_write_types(board);
			//printf("sasiedztwo\n");
			//mx_write_neighbours(board);
		}
	}
	else						// Tryb 8 sasiadow
	{
		count_neighbours_8(board, fold);					// Liczenie sasiadow dla 0 cyklu

		//save_as_bitmap(fopen("e:/ZZ_GOL/b0.bmp", "wb"), board, scale);
		//printf("sasiedztwo\n");
		//mx_write_neighbours(board);

		for (int i = 0; i < numberOfCycles; i++)
		{
			//printf("%d\n", i);
			make_a_cycle_rewrite_struct_8(board, nx, board->c - 1, board->r - 1, fold);
			mx_cpy(nx, board);
			//save_as_bitmap(fopen(pat[i], "wb"), board, scale);

/*			printf("%d-------------- typy\n", i);
			mx_write_types(board);
			printf("sasiedztwo\n");
			mx_write_neighbours(board);*/
		}
	}

	if (bmpOutputFile != NULL)	// Zapis do pliku .bmp
		save_as_bitmap(bmpOutputFile, board, scale);

	if (pngOutputFile != NULL)	// Zapis do pliku .png
	{
		process_png_file(board, scale);
		write_png_file(pngOutputFile);
	}
	mx_free(nx);
	mx_free(board);

	// HACK: Tylko potrzebne mi do odpalania programu w VS
	//getchar();

	exit(0);
}
