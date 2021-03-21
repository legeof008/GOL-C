#ifndef _PNG_CREATOR_H_
#define _PNG_CREATOR_H_

#define MAX_SCALE 10

int width;
int height;
png_byte colorType;
png_byte bitDepth;
png_bytep *rowPointers;
png_structp png;
png_infop info;

int write_png_file(char *filename);

int process_png_file();

#endif
