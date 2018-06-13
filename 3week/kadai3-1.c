#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jpegio.h"


double getPSNR(BYTE *data1, BYTE *data2, int width, int height) {
	int x, y;
	double redSub, greenSub, blueSub, mse;
	double sum = 0;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			redSub = pow(data1[3 * (x + y * width)] - data2[3 * (x + y * width)], 2);
			greenSub = pow(data1[3 * (x + y * width)+1] - data2[3 * (x + y * width)+1], 2);
			blueSub = pow(data1[3 * (x + y * width)+2] - data2[3 * (x + y * width)+2], 2);
			sum += redSub + greenSub + blueSub;
		}
	}

	mse = sum / (3 * width * height);
	return 10 * log10((255 * 255) / mse);
}

int main() {
	char *fileName1 = "jinbutu.jpg";
	char *fileName2 = "haikei.jpg";
	BYTE *imageData1 = NULL;
	BYTE *imageData2 = NULL;
	int imageByte1 = readJpeg(fileName1, &imageData1);
	int imageByte2 = readJpeg(fileName2, &imageData2);

	//同じサイズを使う
	int width = readHeader(fileName1, IMAGE_WIDTH);
	int height = readHeader(fileName1, IMAGE_HEIGHT);
	int bpp = readHeader(fileName1, IMAGE_BPP);

	BYTE *oneData = (BYTE*)malloc(imageByte1);
	BYTE *twoData = (BYTE*)malloc(imageByte1);
	BYTE *sixData = (BYTE*)malloc(imageByte1);
	
	//データに数値代入
	int x, y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			oneData[3 * (x + y * width)] = 1;
			oneData[3 * (x + y * width) + 1] = 1;
			oneData[3 * (x + y * width) + 2] = 1;
			twoData[3 * (x + y * width)] = 2;
			twoData[3 * (x + y * width) + 1] = 2;
			twoData[3 * (x + y * width) + 2] = 2;
			sixData[3 * (x + y * width)] = 6;
			sixData[3 * (x + y * width) + 1] = 6;
			sixData[3 * (x + y * width) + 2] = 6;
		}		
	}

	printf("%f\n", getPSNR(twoData, oneData, width, height));
	printf("%f\n", getPSNR(sixData, oneData, width, height));

	
	return 0;
}
