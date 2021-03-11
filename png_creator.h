#ifndef _PNG_CREATOR_H_
#define _PNG_CREATOR_H_

unsigned char array[][3] = {{255,0,0,},{255,0,0},{0,0,0},{255,255,0},{255,255,0},
                            {255,0,0,},{255,0,0,},{0,0,0},{255,255,0},{255,255,0},
                            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
                            {255,0,0,},{255,0,0,},{0,0,0},{255,255,0},{255,255,0},
                            {255,0,0,},{255,0,0,},{0,0,0},{255,255,0},{255,255,0},};

int width = 5;
int height = 5;
png_byte colorType;
png_byte bitDepth;
png_bytep *rowPointers;
png_structp png;
png_infop info;

void write_png_file(char *filename);

void process_png_file();

#endif
