#ifndef _PNG_CREATOR_H_
#define _PNG_CREATOR_H_

int width;
int height;
png_byte colorType;
png_byte bitDepth;
png_bytep *rowPointers;
png_structp png;
png_infop info;

void write_png_file(char *filename);

void process_png_file();

#endif
