#include "image_creator.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_SCALE 10
#define WALL_COLOR 0x0000FF // Niebieski
#define DEAD_COLOR 0x000000 // Czarny

void write_file_header_to_bitmap(FILE* image, int imageWidth, int imageHeight)
{
    int b = 'B';
    fwrite(&b, 1, 1, image);
    b = 'M';
    fwrite(&b, 1, 1, image);    // Magic number -> okresla typ pliku; dla bitmapy jets to BM
    b = imageWidth * imageHeight * 3 + (imageWidth % 4) * imageHeight + 54;
    fwrite(&b, 4, 1, image);    // Wielkosc calego pliku
    b = 0;
    fwrite(&b, 4, 1, image);    // Nie jest wazne -> powinno byc 0
    b = 54;
    fwrite(&b, 4, 1, image);    // Offset listy pixeli
}

void write_info_header_to_bitmap(FILE* image, int imageWidth, int imageHeight)
{
    int b = 40;
    fwrite(&b, 4, 1, image);    // Wielkosc tego naglowka (40, jesli bez palety kolorow)
    b = imageWidth;
    fwrite(&b, 4, 1, image);    // Szerokosc obrazu
    b = imageHeight;
    fwrite(&b, 4, 1, image);    // Wysokosc obrazu
    b = 1;
    fwrite(&b, 2, 1, image);    // Liczba plaszczyzn (powinno byc 1)
    b = 24;
    fwrite(&b, 2, 1, image);    // Ilosc bitow na 1 piksel (24->3*1B (B, G, R))
    b = 0;
    fwrite(&b, 4, 1, image);    // Rodzaj kompresji
    b = 0;
    fwrite(&b, 4, 1, image);    // Wielkosc obrazu po kompresji (gdy rodzaj kompresji jest 0, to powinno byc 0)
    b = 0;
    fwrite(&b, 4, 1, image);    // Pozioma rozdzielczosc piksele/metr (nie jest wazne -> powinno byc 0)
    b = 0;
    fwrite(&b, 4, 1, image);    // Pionowa rozdzielczosc piksele/metr (nie jest wazne -> powinno byc 0)
    b = 0;
    fwrite(&b, 4, 1, image);    // Liczba uzytych kolorow w palecie(nie jest wazne -> powinno byc 0)
    b = 0;
    fwrite(&b, 4, 1, image);    // Liczba "waznych" kolorow (nie jest wazne -> powinno byc 0)
}

int save_as_bitmap(char* path, Matrix *matrix, int scale)
{
    if (scale >= MAX_SCALE || scale < 1)
    {
        fprintf(stderr, "Nie mozna utworzyc obrazu (podana skala jest zla. Powinna miescic sie w (1, %d))", MAX_SCALE);
        return -1;
    }

    FILE* image = fopen(path, "wb");

    if (image == NULL)
    {
        fprintf(stderr, "Nie mozna utworzyc obrazu (problem ze sciezka)");
        return -1;
    }

    int sw = scale * matrix->c;
    int sh = scale * matrix->r;

    write_file_header_to_bitmap(image, sw, sh);
    write_info_header_to_bitmap(image, sw, sh);

    int b = 0;  // Buffer do zapisywania za pomoca fwrite
    // Zapisywanie kolorow pikseli
    for (int i = matrix->r - 1; i >= 0; i--)    // Kolumny
    {
        for (int si = 0; si < scale; si++)
        {
            for (int j = 0; j < matrix->c; j++)    // Wiersze
            {
                for (int sj = 0; sj < scale; sj++)
                {
                    if (mx_get_single_val(matrix, i, j, 't') == 0)  // Martwa komorka
                    {
                        b = DEAD_COLOR;
                        fwrite(&b, 3, 1, image);
                    }
                    else if (mx_get_single_val(matrix, i, j, 't') == 2) // Sciana
                    {
                        b = WALL_COLOR;
                        fwrite(&b, 3, 1, image);
                    }
                    else
                    {
                        b = mx_get_single_val(matrix, i, j, 'b');
                        fwrite(&b, 1, 1, image);        // Blue
                        b = mx_get_single_val(matrix, i, j, 'g');
                        fwrite(&b, 1, 1, image);        // Green
                        b = mx_get_single_val(matrix, i, j, 'r');
                        fwrite(&b, 1, 1, image);        // Red
                    }
                }
            }
            // Dopisywanie kilku 0 do konca wiersza
            b = 0;
            fwrite(&b, 1, sw % 4, image);
        }
    }

    fclose(image);

    return 0;
}

void read_bitmap(char* bitmapPath, char* outputPath)
{
    FILE *bitmap = fopen(bitmapPath, "rb");
    FILE* output = fopen(outputPath, "w");

    int b;
    fread(&b, 1, 1, bitmap);
    fprintf(output, "%c", b);
    fread(&b, 1, 1, bitmap);
    fprintf(output, "%c ", b);

    for (int i = 0; i < 13; i++)
    {
        if (i == 6)
        {
            fread(&b, 2, 1, bitmap);
            fprintf(output, "%u ", b);
            fread(&b, 2, 1, bitmap);
            fprintf(output, "%u ", b);
        }
        else
        {
            fread(&b, 4, 1, bitmap);
            fprintf(output, "%u ", b);
        }
    }
}
