#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include "png_creator.h"

void write_png_file(char *filename)
{
  FILE *fp = fopen(filename, "wb");
  if(!fp) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGB,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );

  if(bitDepth == 16)
    png_set_strip_16(png);

  if(colorType == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  png_set_expand(png);
  png_write_info(png, info);
  png_write_image(png, rowPointers);
  png_write_end(png, NULL);


  for(unsigned int y = 0; y < height; y++)
  {
    free(rowPointers[y]);
  }
  free(rowPointers);

  fclose(fp);
}

void init()
{

}

void process_png_file()
{
  png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  info = png_create_info_struct(png);
  if (!info) abort();

  rowPointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++)
  {
    rowPointers[y] = (png_byte*)malloc(sizeof(png_byte)*width*3); // SPRAWDZIC: ile tu trzeba wpisac? wczesniej bylo: png_get_rowbytes(png,info));
  }

  for (unsigned int y = 0; y < height; y++)
  {
      for (unsigned int x = 0; x < width; x++)
      {
        png_bytep px = &(rowPointers[y][x * 3]);

          for(int i = 0; i < 3; ++i)
            px[i] = array[y*width + x][i];
      }
  }
}


int main(int argc, char *argv[])
{

  if(argc < 2)
    fprintf(stderr, "Podaj nazwe nowo tworzonego pliku .png!\n");

  process_png_file();
  write_png_file(argv[1]);

  return 0;
}
