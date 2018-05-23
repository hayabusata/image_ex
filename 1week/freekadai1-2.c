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

	int hist[256][width * height];
	int x, y;
	int a;
	for (x = 0; x < 256; x++) {
		for (y = 0; y < width * height; y++) {
			hist[x][y] = 0;
		}
	}

	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			a = data[3 * (x + y * width)];
			hist[a][x + y * width] = 1;
		}
	}

	for (x = 0; x < 256; x++) {
		for (y = 0; y < width * height; y++) {
			printf("%d", hist[x][y]);
		}
		printf("\n");
	}

	/*if (writeJpeg("free1-2result.jpg", hist, 256, width * height, 1, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}*/

	return 0;
}
