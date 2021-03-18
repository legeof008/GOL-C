#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include "cell.h"
#include "png_creator.h"

void write_png_file(char *filename)
{
  FILE *fp = fopen(filename, "wb");
  if(!fp)
    fprintf(stderr, "[write_png_file] Nie udalo otworzyc sie pliku %s do zapisu", filename);

  png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png)
    fprintf(stderr, "[write_png_file] png_create_write_struct: nie powiodlo sie");

  info = png_create_info_struct(png);
  if (!info)
    fprintf(stderr, "[write_png_file] png_create_info_struct: nie powiodlo sie");

  if (setjmp(png_jmpbuf(png)))
    fprintf(stderr, "[write_png_file] wystapil blad podczas init_io");

  png_init_io(png, fp);

  if (setjmp(png_jmpbuf(png)))
    fprintf(stderr, "[write_png_file] blad w trakcie tworzenia naglowka");

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
    fprintf(stderr, "[write_png_file] blad przy zapisie bajtow");

  png_write_image(png, rowPointers);
  if (setjmp(png_jmpbuf(png)))
    fprintf(stderr, "[write_png_file] blad przy koncu zapisu");

  png_write_end(png, NULL);

  for(unsigned int y = 0; y < height; y++)
    free(rowPointers[y]);
  free(rowPointers);

  fclose(fp);
}


void process_png_file(Matrix *matrix)
{
  width = matrix->c;
  height = matrix->r;
  bitDepth = 8;
  colorType = PNG_COLOR_TYPE_RGB;

  rowPointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++)
  {
    rowPointers[y] = (png_byte*)malloc(sizeof(png_byte)*width*3); // SPRAWDZIC: ile tu trzeba wpisac? wczesniej bylo: png_get_rowbytes(png,info));
  }

  for (unsigned int y = 0; y < height; y++)
  {
      for (unsigned int x = 0; x < width; x++)
      {
        png_bytep px = &(rowPointers[height -1 - y][x * 3]);

        px[0] = matrix->data[y*height + x].R;
        px[1] = matrix->data[y*height + x].G;
        px[2] = matrix->data[y*height + x].B;
      }
  }
}


int main(int argc, char *argv[])
{
  Matrix matrix;
  matrix.r = 100;
  matrix.c = 100;
  matrix.data = (Cell*)malloc(matrix.r*matrix.c*sizeof(Cell));
  for(int i = 0 ; i < 100 ; i++)
  {
      matrix.data[i].G = (char)255;
  }
  for(int i = 0 ; i < 100 ; i++)
  {
      matrix.data[99*matrix.r+i].R = (char)255;
  }


  if(argc < 2)
    fprintf(stderr, "Podaj nazwe nowo tworzonego pliku .png!\n");

  process_png_file(&matrix);
  write_png_file(argv[1]);

  return 0;
}
