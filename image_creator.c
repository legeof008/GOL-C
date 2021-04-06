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

void save_as_bitmap(FILE* file, Matrix* matrix, int scale)
{
	int sw, sh;
	int b;
	int i, si, j, sj;

	sw = scale * matrix->c;
	sh = scale * matrix->r;

	write_file_header_to_bitmap(file, sw, sh);
	write_info_header_to_bitmap(file, sw, sh);

	b = 0;  // Buffer do zapisywania za pomoca fwrite
	// Zapisywanie kolorow pikseli
	for (i = 0; i < matrix->r; i++)    // Kolumny
	{
		for (si = 0; si < scale; si++)
		{
			for (j = 0; j < matrix->c; j++)    // Wiersze
			{
				for (sj = 0; sj < scale; sj++)
				{
					if (mx_get_single_val(matrix, i, j, 't') == 0)  // Martwa komorka
					{
						b = DEAD_COLOR;
						fwrite(&b, 3, 1, file);
					}
					else if (mx_get_single_val(matrix, i, j, 't') == 2) // Sciana
					{
						b = WALL_COLOR;
						fwrite(&b, 3, 1, file);
					}
					else
					{
						b = mx_get_single_val(matrix, i, j, 'b');
						fwrite(&b, 1, 1, file);        // Blue
						b = mx_get_single_val(matrix, i, j, 'g');
						fwrite(&b, 1, 1, file);        // Green
						b = mx_get_single_val(matrix, i, j, 'r');
						fwrite(&b, 1, 1, file);        // Red
					}
				}
			}
			// Dopisywanie kilku 0 do konca wiersza
			b = 0;
			fwrite(&b, 1, sw % 4, file);
		}
	}

	fclose(file);	// TODO: Nwm czy powinno zamykac sie tu strumien
}

int write_png_file(FILE *fp)
{
  if (!fp)
  {
    fprintf(stderr, "[write_png_file] Nie udalo otworzyc sie pliku do zapisu\n");
    return -1;
  }

  png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png)
  {
    fprintf(stderr, "[write_png_file] png_create_write_struct: nie powiodlo sie\n");
    return -2;
  }

  info = png_create_info_struct(png);
  if (!info)
  {
    fprintf(stderr, "[write_png_file] png_create_info_struct: nie powiodlo sie\n");
    return -3;
  }

  if (setjmp(png_jmpbuf(png)))
  {
    fprintf(stderr, "[write_png_file] wystapil blad podczas init_io\n");
    return -4;
  }

  png_init_io(png, fp);

  if (setjmp(png_jmpbuf(png)))
  {
    fprintf(stderr, "[write_png_file] blad w trakcie tworzenia naglowka\n");
    return -5;
  }

  png_set_IHDR(
    png,
    info,
    width, height,
    bitDepth,
    colorType,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );

  png_write_info(png, info);
  if (setjmp(png_jmpbuf(png)))
  {
    fprintf(stderr, "[write_png_file] blad przy zapisie bajtow\n");
    return -6;
  }

  png_write_image(png, rowPointers);
  if (setjmp(png_jmpbuf(png)))
  {
    fprintf(stderr, "[write_png_file] blad przy koncu zapisu\n");
    return -7;
  }

  png_write_end(png, NULL);

  for (unsigned int y = 0; y < height; y++)
    free(rowPointers[y]);
  free(rowPointers);

  fclose(fp);
}


void process_png_file(Matrix *matrix, int scale)
{
  width = scale * matrix->c;
  height = scale * matrix->r;
  bitDepth = 8;
  colorType = PNG_COLOR_TYPE_RGB;

  rowPointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for (int y = 0; y < height; y++)
  {
    rowPointers[y] = (png_byte*)malloc(sizeof(png_byte) * width * 3);
  }

  for (unsigned int y = 0; y < matrix->r; y++)
  {
    for (unsigned int x = 0; x < matrix->c; x++)
    {
      for (unsigned int sy = 0; sy < scale; sy++)
      {
        for (unsigned int sx = 0; sx < scale; sx++)
        {
          png_bytep px = &(rowPointers[height - 1 - (scale * y + sy)][(scale * x + sx) * 3]);

          if (mx_get_single_val(matrix, y, x, 't') == 0)  // martwa komorka
          {
            for(int i = 0 ; i < 3 ; i++)
              px[i] = 0;
          }
          else if (mx_get_single_val(matrix, y, x, 't') == 2) // sciana
          {
            px[0] = 0;
            px[1] = 0;
            px[2] = 255;
          }
          else
          {
            px[0] = mx_get_single_val(matrix, y, x, 'r');
            px[1] = mx_get_single_val(matrix, y, x, 'g');
            px[2] = mx_get_single_val(matrix, y, x, 'b');
          }
        }
      }
    }
  }
}
