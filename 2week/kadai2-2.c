#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jpegio.h"

int main() {
	char *fileName1 = "jinbutu.jpg";
	char *fileName2 = "haikei.jpg";
	char *fileName3 = "kuromaki.jpg";
	BYTE *jinbutuData = NULL;
	BYTE *haikeiData = NULL;
	BYTE *kuromakiData = NULL;
	int jinbutuImageByte = readJpeg(fileName1, &jinbutuData);
	int haikeiImageByte = readJpeg(fileName2, &haikeiData);
	int kuromakiImageByte = readJpeg(fileName3, &kuromakiData);

	//同じサイズを使う
	int width = readHeader(fileName1, IMAGE_WIDTH);
	int height = readHeader(fileName1, IMAGE_HEIGHT);
	int bpp = readHeader(fileName1, IMAGE_BPP);

	//maskの初期化
	BYTE *mask = (BYTE*)malloc(jinbutuImageByte); 
	int x, y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			mask[3 * (x + y * width)] = 255;
			mask[3 * (x + y * width) + 1] = 255;
			mask[3 * (x + y * width) + 2] = 255;
		}
	}

	//クロマキー画像の平均値取得
	int redSum = 0, greenSum = 0, blueSum = 0;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			redSum += kuromakiData[3 * (x + y * width)];
			greenSum += kuromakiData[3 * (x + y * width) + 1];
			blueSum += kuromakiData[3 * (x + y * width) + 2];		
		}
	}

	double redAverage = redSum / (double)(width * height);
	double greenAverage = greenSum / (double)(width * height);
	double blueAverage = blueSum / (double)(width * height);

	//青色判別
	int d = 84;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			if (abs((double)jinbutuData[3 * (x + y * width)] - redAverage) < d
			    && abs((double)jinbutuData[3 * (x + y * width) + 1] - greenAverage) < d 
			    && abs((double)jinbutuData[3 * (x + y * width) + 2] - blueAverage) < d ) {
				mask[3 * (x + y * width)] = 0;
				mask[3 * (x + y * width) + 1] = 0;
				mask[3 * (x + y * width) + 2] = 0;
			}
		}
	}

	char outFileName[14];
	sprintf(outFileName, "result2-2-%d.jpg", d);
	if (writeJpeg(outFileName, mask, width, height, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	free(mask);

	return 0;
}
