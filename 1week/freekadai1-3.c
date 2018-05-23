#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "jpegio.h"

int main() {
	char *fileName = "testImage.jpg";
	BYTE *data = NULL;
	int imageByte = readJpeg(fileName, &data);
	int width = readHeader(fileName, IMAGE_WIDTH);
	int height = readHeader(fileName, IMAGE_HEIGHT);
	int bpp = readHeader(fileName, IMAGE_BPP);

	BYTE *copyData = (BYTE*)malloc(imageByte);
	int x, y;
	double rFactor = 1.1, gFactor = 1.1, bFactor = 1.1;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			if (rFactor * copyData[3 * (x + y * width)] > 255) copyData[3 * (x + y * width)] = 255;
			else copyData[3 * (x + y * width)] = rFactor * (double)data[3 * (x + y * width)];
			if (gFactor * copyData[3 * (x + y * width) + 1] > 255) copyData[3 * (x + y * width) + 1] = 255;
			else copyData[3 * (x + y * width) + 1] = gFactor * (double)data[3 * (x + y * width) + 1];
			if (bFactor * copyData[3 * (x + y * width) + 2] > 255) copyData[3 * (x + y * width) + 2] = 255;
			else copyData[3 * (x + y * width) + 2] = bFactor * (double)data[3 * (x + y * width) + 2];
			
		}
	}

	if (writeJpeg("free1-3result.jpg", copyData, width, height, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	free(copyData);

	return 0;
}
