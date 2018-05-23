#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "jpegio.h"

typedef struct _Image
{
	BYTE *data;
	int imageByte;
	int width;
	int height;
	int bpp;
} _Image;

int main() {
	char *fileName = "testImage.jpg";
	_Image inputFile;
	inputFile->data = NULL;
	inputFile.imageByte = readJpeg(fileName, inputFile.data);
	inputFile.width = readHeader(fileName, IMAGE_WIDTH);
	inputFile.height = readHeader(fileName, IMAGE_HEIGHT);
	inputFile.bpp = readHeader(fileName, IMAGE_BPP);

	_Image outFile;
	outFile.data = (BYTE*)malloc(inputFile.imageByte);
	int x, y;
	for (x = 0; x < inputFile.width; x++) {
		for (y = 0; y < inputFile.height; y++) {
			outFile.data[3 * (x + y * inputFile.width)] = inputFile.data[3 * (x + y * inputFile.width)];
			outFile.data[3 * (x + y * inputFile.width) + 1] = inputFile.data[3 * (x + y * inputFile.width) + 1];
			outFile.data[3 * (x + y * inputFile.width) + 2] = inputFile.data[3 * (x + y * inputFile.width) + 2];
		}
	}

	if (writeJpeg("free1-4result_quality100.jpg", outFile.data, inputFile.width, inputFile.height, inputFile.bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	if (writeJpeg("free1-4result_quality1.jpg",  outFile.data, inputFile.width, inputFile.height, inputFile.bpp, 1) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	free(inputFile.data);
	free(outFile.data);

	return 0;
}
