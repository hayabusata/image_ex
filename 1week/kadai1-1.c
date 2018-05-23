#include<stdio.h>
#include<stdlib.h>
#include "jpegio.h"

int main() {
	char *fileName = "testImage.jpg";
	int width = readHeader(fileName, IMAGE_WIDTH);
	int height = readHeader(fileName, IMAGE_HEIGHT);
	int bpp = readHeader(fileName, IMAGE_BPP);
	int imageSize = width * height * bpp;
	printf("width = %d, height = %d, bpp = %d, imageSize = %d\n", width, height, bpp, imageSize);

	return 0;
}
