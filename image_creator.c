#include "image_creator.h"

#include <stdio.h>
#include <stdlib.h>

// TODO: Lepsze zapisywanie naglowkow do pliku (moze bez zmiennej b; usunac niepotrzebne przypisania wartosci b)
void write_file_header_to_bitmap(FILE* image, int imageWidth, int imageHeight)
{
    int b = 'B';
    fwrite(&b, 1, 1, image);
    b = 'M';
    fwrite(&b, 1, 1, image);    // Magic number -> okresla typ pliku; dla bitmapy jets to BM
    b = imageWidth * imageHeight * 3 + (imageWidth % 4) * imageHeight + 54;
    fwrite(&b, 4, 1, image);    // Wielkosc calego pliku
    b = 0;
    fwrite(&b, 4, 1, image);    // Bity w rezerwie
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
    fwrite(&b, 2, 1, image);    // Numer płaszczyzny obrazu, dla bmp MUSI BYĆ 1 koniecznie 
    b = 24;
    fwrite(&b, 2, 1, image);    // Ilosc bitow na 1 piksel (24->3*1B (B, G, R))
    b = 0;
    fwrite(&b, 4, 1, image);    // Rodzaj kompresji
    b = 0;
    fwrite(&b, 4, 1, image);    // Wielkosc obrazu po kompresji (gdy rodzaj kompresji jest 0, to powinno byc 0)
    b = imageWidth * imageHeight * 3 + (imageWidth % 4) * imageHeight;
    fwrite(&b, 4, 1, image);    // Pozioma rozdzielczosc piksele/metr (nie jest wazne -> powinno byc 0)
    b = 0;
    fwrite(&b, 4, 1, image);    // Pionowa rozdzielczosc piksele/metr (nie jest wazne -> powinno byc 0)
    b = 0;
    fwrite(&b, 4, 1, image);    // Liczba uzytych kolorow w palecie(nie jest wazne -> powinno byc 0)
    b = 0;
    fwrite(&b, 4, 1, image);    // TODO: Opisac
}

// TODO: Zwracanie true/false -> udalo sie zapisac / nie udalo sie
// TODO: Skala obrazu??
// TODO: Obrot obrazu -> na razie obraz jest obrocony
void save_as_bitmap(char* path, Matrix *matrix)
{
    FILE* image = fopen(path, "wb");

    //TODO: Sprawdzanie, czy sciezka istnieje i tworzenie sciezki

    if (image == NULL)
    {
        fprintf(stderr, "Nie mozna utworzyc obrazu (problem ze sciezka)");
        return;
    }

    write_file_header_to_bitmap(image, matrix->c, matrix->r);
    write_info_header_to_bitmap(image, matrix->c, matrix->r);

    int b = 0;  // Buffer do zapisywania za pomoca fwrite
    // Zapisywanie kolorow pikseli
    for (int i = 0; i < matrix->r; i++)
    {
        for (int j = 0; j < matrix->c; j++)
        {
            b = mx_get_single_val(matrix, i, j, 'b');
            fwrite(&b, 1, 1, image);        // Blue
            b = mx_get_single_val(matrix, i, j, 'g');
            fwrite(&b, 1, 1, image);        // Green
            b = mx_get_single_val(matrix, i, j, 'r');
            fwrite(&b, 1, 1, image);        // Red
        }
        // Dopisywanie kilku 0 do konca wiersza
        b = 0;
        fwrite(&b, 1, matrix->c % 4, image);
    }

    fclose(image);
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
