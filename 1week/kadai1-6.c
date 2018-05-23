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

	
	BYTE *monoData1 = (BYTE*)malloc(imageByte);
	BYTE *monoData2 = (BYTE*)malloc(imageByte);
	int x, y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			monoData1[3 * (x + y * width)] = 
				(data[3 * (x + y * width)] 
				+ data[3 * (x + y * width) + 1]
				+ data[3 * (x + y * width) + 2]) / 3;
			monoData1[3 * (x + y * width) + 1] = 
				(data[3 * (x + y * width)] 
				+ data[3 * (x + y * width) + 1] 
				+ data[3 * (x + y * width) + 2]) / 3;
			monoData1[3 * (x + y * width) + 2] = 
				(data[3 * (x + y * width)] 
				+ data[3 * (x + y * width) + 1] 
				+ data[3 * (x + y * width) + 2]) / 3;

			monoData2[3 * (x + y * width)] = 
				0.299 * data[3 * (x + y * width)]
				+ 0.587 * data[3 * (x + y * width) + 1]
				+ 0.114 * data[3 * (x + y * width) + 2];
			monoData2[3 * (x + y * width) + 1] = 
				0.299 * data[3 * (x + y * width)]
				+ 0.587 * data[3 * (x + y * width) + 1]
				+ 0.114 * data[3 * (x + y * width) + 2];
			monoData2[3 * (x + y * width) + 2] = 
				0.299 * data[3 * (x + y * width)]
				+ 0.587 * data[3 * (x + y * width) + 1]
				+ 0.114 * data[3 * (x + y * width) + 2];
		}
	}

	if (writeJpeg("1-6result_4-2.jpg", monoData1, width, height, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	if (writeJpeg("1-6result_4-3.jpg", monoData2, width, height, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	free(monoData1);
	free(monoData2);

	return 0;
}
