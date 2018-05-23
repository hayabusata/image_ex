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

	BYTE *monoData = (BYTE*)malloc(imageByte);
	int x, y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			monoData[3 * (x + y * width)] = 
				0.299 * data[3 * (x + y * width)]
				+ 0.587 * data[3 * (x + y * width) + 1]
				+ 0.114 * data[3 * (x + y * width) + 2];
			if (monoData[3 * (x + y * width)] > 127) {
				monoData[3 * (x + y * width)] = 255;
			} else {
				monoData[3 * (x + y * width)] = 0;
			}	

			monoData[3 * (x + y * width) + 1] = 
				0.299 * data[3 * (x + y * width)]
				+ 0.587 * data[3 * (x + y * width) + 1]
				+ 0.114 * data[3 * (x + y * width) + 2];
			if (monoData[3 * (x + y * width) + 1] > 127) {
				monoData[3 * (x + y * width) + 1] = 255;
			} else {
				monoData[3 * (x + y * width) + 1] = 0;	
			}

			monoData[3 * (x + y * width) + 2] = 
				0.299 * data[3 * (x + y * width)]
				+ 0.587 * data[3 * (x + y * width) + 1]
				+ 0.114 * data[3 * (x + y * width) + 2];
			if (monoData[3 * (x + y * width) + 2] > 127) {
				monoData[3 * (x + y * width) + 2] = 255;
			} else {
				monoData[3 * (x + y * width) + 2] = 0;
			}
		}
	}

	if (writeJpeg("free1-1result.jpg", monoData, width, height, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	free(monoData);

	return 0;
}
