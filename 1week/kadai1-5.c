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

	
	BYTE *redData = (BYTE*)malloc(imageByte);
	int x, y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			redData[3 * (x + y * width)] = data[3 * (x + y * width)];
			redData[3 * (x + y * width) + 1] = 0;
			redData[3 * (x + y * width) + 2] = 0;
		}
	}

	BYTE *greenData = (BYTE*)malloc(imageByte);
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			greenData[3 * (x + y * width)] = 0;
			greenData[3 * (x + y * width) + 1] = data[3 * (x + y * width) + 1];
			greenData[3 * (x + y * width) + 2] = 0;
		}
	}

	BYTE *blueData = (BYTE*)malloc(imageByte);
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			blueData[3 * (x + y * width)] = 0;
			blueData[3 * (x + y * width) + 1] = 0;
			blueData[3 * (x + y * width) + 2] = data[3 * (x + y * width) + 2];
		}
	}

	if (writeJpeg("1-5result_red.jpg", redData, width, height, bpp, 100) == 1) {
		printf("red success\n");
	} else {
		printf("not success\n");
	}

	if (writeJpeg("1-5result_green.jpg", greenData, width, height, bpp, 100) == 1) {
		printf("green success\n");
	} else {
		printf("not success\n");
	}

	if (writeJpeg("1-5result_blue.jpg", blueData, width, height, bpp, 100) == 1) {
		printf("blue success\n");
	} else {
		printf("not success\n");
	}

	free(redData);
	free(greenData);
	free(blueData);

	return 0;
}
