#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include "cell.h"
#include "png_creator.h"

int write_png_file(char *filename)
{
  FILE *fp = fopen(filename, "wb");
  if (!fp)
  {
    fprintf(stderr, "[write_png_file] Nie udalo otworzyc sie pliku %s do zapisu\n", filename);
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


int process_png_file(Matrix *matrix, int scale)
{
  if (scale >= MAX_SCALE || scale < 1)
  {
    fprintf(stderr, "Nie mozna utworzyc obrazu (podana skala jest zla. Powinna miescic sie w (1, %d))\n", MAX_SCALE);
    return -1;
  }

  width = scale * matrix->c;
  height = scale * matrix->r;
  bitDepth = 8;
  colorType = PNG_COLOR_TYPE_RGB;

  rowPointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for (int y = 0; y < height; y++)
  {
    rowPointers[y] = (png_byte*)malloc(sizeof(png_byte) * width * 3); // SPRAWDZIC: ile tu trzeba wpisac? wczesniej bylo: png_get_rowbytes(png,info));
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
  return 0;
}

/*
int main(int argc, char *argv[])
{
  if(argc < 2)
    fprintf(stderr, "Podaj nazwe nowo tworzonego pliku .png!\n");

  Matrix matrix;
  matrix.r = 10;
  matrix.c = 10;
  matrix.data = (Cell*)malloc(matrix.r*matrix.c*sizeof(Cell));

  matrix.data[0].type = 1;
  matrix.data[0].R = 255;

  for (int i = 1; i < 10*10; i++)
    matrix.data[i].type = 2;

  if (process_png_file(&matrix, 9) < 0)
    return -1;

  if (write_png_file(argv[1]) < 0)
    return -2;

  return 0;
}
*/
