#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "jpegio.h"

int main() {
	char *fileName = "haikei.jpg";
	BYTE *data = NULL;
	int imageByte = readJpeg(fileName, &data);
	int width = readHeader(fileName, IMAGE_WIDTH);
	int height = readHeader(fileName, IMAGE_HEIGHT);
	int bpp = readHeader(fileName, IMAGE_BPP);

	BYTE *copyData = (BYTE*)malloc(imageByte);
	int x, y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			copyData[3 * (x + y * width)] = data[3 * (x + y * width)];
			copyData[3 * (x + y * width) + 1] = data[3 * (x + y * width) + 1];
			copyData[3 * (x + y * width) + 2] = data[3 * (x + y * width) + 2];
		}
	}

	if (writeJpeg("3-2result_quality1.jpg", copyData, width, height, bpp, 1) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	if (writeJpeg("3-2result_quality5.jpg", copyData, width, height, bpp, 5) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	if (writeJpeg("3-2result_quality10.jpg", copyData, width, height, bpp, 10) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	if (writeJpeg("3-2result_quality20.jpg", copyData, width, height, bpp, 20) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	if (writeJpeg("3-2result_quality40.jpg", copyData, width, height, bpp, 40) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	if (writeJpeg("3-2result_quality80.jpg", copyData, width, height, bpp, 80) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	free(copyData);

	return 0;
}
