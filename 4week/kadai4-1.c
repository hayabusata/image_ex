#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jpegio.h"

int n = 9;

//9つのデータの中央値
int getMedian(int *data) {
	int i, j, k;
	for (i = 0; i < n - 1; i++) {
		j = i;
		for (k = i; k < n; k++) {
			if (data[k] < data[j]) j = k;
		}
		if (i < j) {
			int v = data[i];
			data[i] = data[j];
			data[j] = v;
		}
	}

	return data[n / 2];
}

int main() {
	char *fileName = "noise2_s.jpg";
	BYTE *imageData = NULL;
	int imageByte = readJpeg(fileName, &imageData);

	//同じサイズを使う
	int width = readHeader(fileName, IMAGE_WIDTH);
	int height = readHeader(fileName, IMAGE_HEIGHT);
	int bpp = readHeader(fileName, IMAGE_BPP);

	BYTE twoDimData[width][height];
	BYTE *medianData = (BYTE*)malloc(imageByte);
	int areaData[n];
	
	//RED
	int x, y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			twoDimData[x][y] = imageData[3 * (x + y * width)];
		}
	}
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			areaData[0] = twoDimData[x][y];
			if (x - 1 < 0) {
				areaData[1] = 0;
			} else {
				areaData[1] = twoDimData[x - 1][y];
			}
			if (x + 1 >= width) {
				areaData[2] = 0;
			} else {
				areaData[2] = twoDimData[x + 1][y];
			}
			if (y - 1 < 0) {
				areaData[3] = 0;
			} else {
				areaData[3] = twoDimData[x][y - 1];
			}
			if (y + 1 >= height) {
				areaData[4] = 0;
			} else {
				areaData[4] = twoDimData[x][y + 1];
			}
			if (x - 1 < 0 || y - 1 < 0) {
				areaData[5] = 0;
			} else {
				areaData[5] = twoDimData[x - 1][y - 1];
			}
			if (x + 1 >= width || y - 1 < 0) {
				areaData[6] = 0;
			} else {
				areaData[6] = twoDimData[x + 1][y - 1];
			}
			if (x - 1 < 0 || y + 1 >= height) {
				areaData[7] = 0;
			} else {
				areaData[7] = twoDimData[x - 1][y + 1];
			}
			if (x + 1 >= width || y + 1 >= height) {
				areaData[8] = 0;
			} else {
				areaData[8] = twoDimData[x + 1][y + 1];
			}

			medianData[3 * (x + y * width)] = getMedian(areaData);
			
		}
	}

	

	//GREEN
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			twoDimData[x][y] = imageData[3 * (x + y * width) + 1];
		}
	}
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			areaData[0] = twoDimData[x][y];
			if (x - 1 < 0) {
				areaData[1] = 0;
			} else {
				areaData[1] = twoDimData[x - 1][y];
			}
			if (x + 1 >= width) {
				areaData[2] = 0;
			} else {
				areaData[2] = twoDimData[x + 1][y];
			}
			if (y - 1 < 0) {
				areaData[3] = 0;
			} else {
				areaData[3] = twoDimData[x][y - 1];
			}
			if (y + 1 >= height) {
				areaData[4] = 0;
			} else {
				areaData[4] = twoDimData[x][y + 1];
			}
			if (x - 1 < 0 || y - 1 < 0) {
				areaData[5] = 0;
			} else {
				areaData[5] = twoDimData[x - 1][y - 1];
			}
			if (x + 1 >= width || y - 1 < 0) {
				areaData[6] = 0;
			} else {
				areaData[6] = twoDimData[x + 1][y - 1];
			}
			if (x - 1 < 0 || y + 1 >= height) {
				areaData[7] = 0;
			} else {
				areaData[7] = twoDimData[x - 1][y + 1];
			}
			if (x + 1 >= width || y + 1 >= height) {
				areaData[8] = 0;
			} else {
				areaData[8] = twoDimData[x + 1][y + 1];
			}

			medianData[3 * (x + y * width) + 1] = getMedian(areaData);
		}
	}

	//BLUE
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			twoDimData[x][y] = imageData[3 * (x + y * width) + 2];
		}
	}
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			areaData[0] = twoDimData[x][y];
			if (x - 1 < 0) {
				areaData[1] = 0;
			} else {
				areaData[1] = twoDimData[x - 1][y];
			}
			if (x + 1 >= width) {
				areaData[2] = 0;
			} else {
				areaData[2] = twoDimData[x + 1][y];
			}
			if (y - 1 < 0) {
				areaData[3] = 0;
			} else {
				areaData[3] = twoDimData[x][y - 1];
			}
			if (y + 1 >= height) {
				areaData[4] = 0;
			} else {
				areaData[4] = twoDimData[x][y + 1];
			}
			if (x - 1 < 0 || y - 1 < 0) {
				areaData[5] = 0;
			} else {
				areaData[5] = twoDimData[x - 1][y - 1];
			}
			if (x + 1 >= width || y - 1 < 0) {
				areaData[6] = 0;
			} else {
				areaData[6] = twoDimData[x + 1][y - 1];
			}
			if (x - 1 < 0 || y + 1 >= height) {
				areaData[7] = 0;
			} else {
				areaData[7] = twoDimData[x - 1][y + 1];
			}
			if (x + 1 >= width || y + 1 >= height) {
				areaData[8] = 0;
			} else {
				areaData[8] = twoDimData[x + 1][y + 1];
			}

			medianData[3 * (x + y * width) + 2] = getMedian(areaData);
		}
	}

	for (y = 0; y < 10; y++) {
		printf("%d\n", medianData[3 * y]);
	}

	if (writeJpeg("4-1result.jpg", medianData, width, height, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	
	return 0;
}
