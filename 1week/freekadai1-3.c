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

	int max = 0, w = 0, h = 0;
	int x, y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			if (data[3 * (x + y * width)] + data[3 * (x + y * width) + 1] 
				+ data[3 * (x + y * width) + 2] > max) {
				max = data[3 * (x + y * width)] + data[3 * (x + y * width) + 1] 
					+ data[3 * (x + y * width) + 2];
				w = x;
				h = y;
			}
		}
	}

	double average = ((int)data[3 * (w + h * width)] + (int)data[3 * (w + h * width) + 1] 
			+ (int)data[3 * (w + h * width) + 2]) / 3;

	BYTE *copyData = (BYTE*)malloc(imageByte);
	double rFactor = average / (double)data[3 * (w + h * width)];
	double gFactor = average / (double)data[3 * (w + h * width) + 1];
	double bFactor = average / (double)data[3 * (w + h * width) + 2];

	printf("%f %f %f %f\n", rFactor, gFactor, bFactor, (double)data[3 * (w + h * width)]);

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
