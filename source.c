#include<stdio.h>
#include"image_creator.h"
#include"cell.h"
#include<string.h>
#include<stdlib.h>

int main(int argc,char** argv)
{
    Matrix* mx = mx_read_from_file(argv[1]);
    char* pathname = malloc(sizeof(argv[2])+ 5*sizeof(char));
    strcpy(pathname,argv[2]);
    strcat(pathname,".bmp");
    save_as_bitmap(pathname,mx);
    return 0;
}