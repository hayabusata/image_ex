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
	char *inputFileName1 = "haikei.jpg";
	char *inputFileName2 = "3-2result_quality80.jpg";
	BYTE *inputImageData1 = NULL;
	BYTE *inputImageData2 = NULL;
	int inputImageByte1 = readJpeg(inputFileName1, &inputImageData1);
	int inputImageByte2 = readJpeg(inputFileName2, &inputImageData2);

	//同じサイズを使う
	int width = readHeader(inputFileName1, IMAGE_WIDTH);
	int height = readHeader(inputFileName1, IMAGE_HEIGHT);
	int bpp = readHeader(inputFileName1, IMAGE_BPP);

	//出力画像の作成
	BYTE *outputImageData = (BYTE*)malloc(inputImageByte1);

	//PSNRの取得
	int x, y;
	double maxPsnr = 0, minPsnr = 10000;
	double sum = 0;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			double red = pow(inputImageData1[3 * (x + y * width)] 							- inputImageData2[3 * (x + y * width)], 2);
			double green = pow(inputImageData1[3 * (x + y * width)+1] 							- inputImageData2[3 * (x + y * width)+1, 2], 2);
			double blue = pow(inputImageData1[3 * (x + y * width)+2] 						- inputImageData2[3 * (x + y * width)+2], 2);
			double mse = (red + green + blue) / 3;
			double psnr = 10 * log10((255 * 255) / mse);

			outputImageData[3 * (x + y * width)] = psnr;
			outputImageData[3 * (x + y * width) + 1] = psnr;
			outputImageData[3 * (x + y * width) + 2] = psnr;

			if (psnr > maxPsnr) maxPsnr = psnr;
			if (psnr < minPsnr) minPsnr = psnr;
			
			sum += psnr;
		}
	}	

	if (writeJpeg("free3-1result.jpg", outputImageData, width, height, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	printf("max psnr = %f\n", maxPsnr);
	printf("min psnr = %f\n", minPsnr);
	printf("%f", getPSNR(inputImageData1, inputImageData2, width, height));

	free(inputImageData1);
	free(inputImageData2);
	free(outputImageData);

	return 0;
}
