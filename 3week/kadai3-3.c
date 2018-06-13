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
	char *inputFileName = "q100_s.jpg";
	char *outputFileName = "noise_s.jpg";
	BYTE *inputImageData = NULL;
	BYTE *outputImageData = NULL;
	int inputImageByte = readJpeg(inputFileName, &inputImageData);
	int outputImageByte = readJpeg(outputFileName, &outputImageData);

	//同じサイズを使う
	int width = readHeader(inputFileName, IMAGE_WIDTH);
	int height = readHeader(inputFileName, IMAGE_HEIGHT);
	int bpp = readHeader(inputFileName, IMAGE_BPP);

	printf("PSNR = %f\n", getPSNR(outputImageData, inputImageData, width, height));

	free(inputImageData);
	free(outputImageData);

	return 0;
}
