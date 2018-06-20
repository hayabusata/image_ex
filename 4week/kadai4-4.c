#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jpegio.h"

int n = 3;

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

//4近傍ラプラシアンフィルタ
int getLap4(int *data) {
	int result = 0;
	result = data[1] + data[3] + data[5] + data[7] - 4 * data[4];
	if (result < 0) return 0;
	else if (result > 255) return 255;
	else return result;
}

//8近傍ラプラシアンフィルタ
int getLap8(int *data) {
	int result = 0;
	int i;
	for (i = 0; i < n * n; i++) {
		if (i != 4) result += data[i];	
	}
	result -= 8 * data[4];
	if (result < 0) return 0;
	else if (result > 255) return 255;
	else return result;
}

//4近傍鮮鋭化フィルタ
int getSharpening4(int *data) {
	int result = 0;
	result = 0 - data[1] - data[3] - data[5] - data[7] + 5 * data[4];
	if (result < 0) return 0;
	else if (result > 255) return 255;
	else return result;
}

//8近傍鮮鋭化フィルタ
int getSharpening8(int *data) {
	int result = 0;
	int i;
	for (i = 0; i < n * n; i++) {
		if (i != 4) result -= data[i];	
	}
	result += 9 * data[4];
	if (result < 0) return 0;
	else if (result > 255) return 255;
	else return result;
}

int main() {
	char *fileName = "q100_s.jpg";
	BYTE *imageData = NULL;
	int imageByte = readJpeg(fileName, &imageData);

	//同じサイズを使う
	int width = readHeader(fileName, IMAGE_WIDTH);
	int height = readHeader(fileName, IMAGE_HEIGHT);
	int bpp = readHeader(fileName, IMAGE_BPP);

	BYTE twoDimData[width][height];
	BYTE *sharpData = (BYTE*)malloc(imageByte);
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

			sharpData[3*(x + y * width)] = getSharpening4(areaData);			
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

			sharpData[3 * (x + y * width) + 1] = getSharpening4(areaData);
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

			sharpData[3 * (x + y * width) + 2] = getSharpening4(areaData);
		}
	}

	for (x = 0; x < 10; x++) {
		printf("%d\n", sharpData[3 * (x + 100 * width)]);
	}

	if (writeJpeg("4-4result.jpg", sharpData, width, height, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	
	return 0;
}
