#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jpegio.h"

int n = 5;

//データの中央値
int getMedian(int *data) {
	int i, j, k;
	for (i = 0; i < (n * n) - 1; i++) {
		j = i;
		for (k = i; k < (n * n); k++) {
			if (data[k] < data[j]) j = k;
		}
		if (i < j) {
			int v = data[i];
			data[i] = data[j];
			data[j] = v;
		}
	}

	return data[(n * n) / 2];
}

int main() {
	char *fileName = "noise_s.jpg";
	BYTE *imageData = NULL;
	int imageByte = readJpeg(fileName, &imageData);

	//同じサイズを使う
	int width = readHeader(fileName, IMAGE_WIDTH);
	int height = readHeader(fileName, IMAGE_HEIGHT);
	int bpp = readHeader(fileName, IMAGE_BPP);

	BYTE twoDimData[width][height];
	BYTE *medianData = (BYTE*)malloc(imageByte);
	int areaData[n * n];
	
	//RED
	int x, y, i, j;
	int c = n / 2;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			twoDimData[x][y] = imageData[3 * (x + y * width)];
		}
	}
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			for (i = 0; i < n; i++) {
				for (j = 0; j < n; j++) {
					if (x + i - c < 0 || x + i - c >= width 
						|| y + j - c < 0 || y + j - c >= height) {
						areaData[i * n + j] = 0;
					} else {
						areaData[i*n+j] = twoDimData[x + i - c][y + j - c];
					}
				}
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
			for (i = 0; i < n; i++) {
				for (j = 0; j < n; j++) {
					if (x + i - c < 0 || x + i - c >= width 
						|| y + j - c < 0 || y + j - c >= height) {
						areaData[i * n + j] = 0;
					} else {
						areaData[i*n+j] = twoDimData[x + i - c][y + j - c];
					}
				}
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
			for (i = 0; i < n; i++) {
				for (j = 0; j < n; j++) {
					if (x + i - c < 0 || x + i - c >= width 
						|| y + j - c < 0 || y + j - c >= height) {
						areaData[i * n + j] = 0;
					} else {
						areaData[i*n+j] = twoDimData[x + i - c][y + j - c];
					}
				}
			}

			medianData[3 * (x + y * width) + 2] = getMedian(areaData);
		}
	}

	for (y = 0; y < 10; y++) {
		printf("%d\n", medianData[3 * y]);
	}

	if (writeJpeg("4-2result.jpg", medianData, width, height, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	
	return 0;
}
