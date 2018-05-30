#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jpegio.h"

int main() {
	char *fileName1 = "jinbutu.jpg";
	char *fileName2 = "haikei.jpg";
	BYTE *jinbutuData = NULL;
	BYTE *haikeiData = NULL;
	int jinbutuImageByte = readJpeg(fileName1, &jinbutuData);
	int haikeiImageByte = readJpeg(fileName2, &haikeiData);

	//同じサイズを使う
	int width = readHeader(fileName1, IMAGE_WIDTH);
	int height = readHeader(fileName1, IMAGE_HEIGHT);
	int bpp = readHeader(fileName1, IMAGE_BPP);

	//決めた割合でたしあわせる
	BYTE *outputData = (BYTE*)malloc(jinbutuImageByte);
	int x, y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			outputData[3 * (x + y * width)] = jinbutuData[3 * (x + y * width)] * 0.5
							+ haikeiData[3 * (x + y * width)] * 0.5;
			outputData[3 * (x + y * width)+1] = jinbutuData[3 * (x + y * width) + 1] * 0.5
							+ haikeiData[3 * (x + y * width) + 1] * 0.5;
			outputData[3 * (x + y * width)+2] = jinbutuData[3 * (x + y * width) + 2] * 0.5
							+ haikeiData[3 * (x + y * width) + 2] * 0.5;
		}
	}

	if (writeJpeg("result2-1.jpg", outputData, width, height, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	free(outputData);

	return 0;
}
