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
	char *inputFileName = "haikei.jpg";
	char *outputFileName1 = "3-2result_quality1.jpg";
	char *outputFileName2 = "3-2result_quality5.jpg";
	char *outputFileName3 = "3-2result_quality10.jpg";
	char *outputFileName4 = "3-2result_quality20.jpg";
	char *outputFileName5 = "3-2result_quality40.jpg";
	char *outputFileName6 = "3-2result_quality80.jpg";
	BYTE *inputImageData = NULL;
	BYTE *outputImageData1 = NULL;
	BYTE *outputImageData2 = NULL;
	BYTE *outputImageData3 = NULL;
	BYTE *outputImageData4 = NULL;
	BYTE *outputImageData5 = NULL;
	BYTE *outputImageData6 = NULL;
	int inputImageByte = readJpeg(inputFileName, &inputImageData);
	int outputImageByte1 = readJpeg(outputFileName1, &outputImageData1);
	int outputImageByte2 = readJpeg(outputFileName2, &outputImageData2);
	int outputImageByte3 = readJpeg(outputFileName3, &outputImageData3);
	int outputImageByte4 = readJpeg(outputFileName4, &outputImageData4);
	int outputImageByte5 = readJpeg(outputFileName5, &outputImageData5);
	int outputImageByte6 = readJpeg(outputFileName6, &outputImageData6);

	//同じサイズを使う
	int width = readHeader(inputFileName, IMAGE_WIDTH);
	int height = readHeader(inputFileName, IMAGE_HEIGHT);
	int bpp = readHeader(inputFileName, IMAGE_BPP);

	//PSNRの取得と表示
	double psnr1 = getPSNR(outputImageData1, inputImageData, width, height);
	double psnr2 = getPSNR(outputImageData2, inputImageData, width, height);
	double psnr3 = getPSNR(outputImageData3, inputImageData, width, height);
	double psnr4 = getPSNR(outputImageData4, inputImageData, width, height);
	double psnr5 = getPSNR(outputImageData5, inputImageData, width, height);
	double psnr6 = getPSNR(outputImageData6, inputImageData, width, height);

	printf("quality1 PSNR = %f\n", psnr1);
	printf("quality5 PSNR = %f\n", psnr2);
	printf("quality10 PSNR = %f\n", psnr3);
	printf("quality20 PSNR = %f\n", psnr4);
	printf("quality40 PSNR = %f\n", psnr5);
	printf("quality80 PSNR = %f\n", psnr6);

	//PSNR値のファイル出力
	FILE *outputFile;
	outputFile = fopen("psnr.txt", "w");
	if (outputFile == NULL) {
		printf("can't open a file");
		exit(1);
	}
	fprintf(outputFile, "%f\n", psnr1);
	fprintf(outputFile, "%f\n", psnr2);
	fprintf(outputFile, "%f\n", psnr3);
	fprintf(outputFile, "%f\n", psnr4);
	fprintf(outputFile, "%f\n", psnr5);
	fprintf(outputFile, "%f\n", psnr6);
	
	fclose(outputFile);	

	free(inputImageData);
	free(outputImageData1);
	free(outputImageData2);
	free(outputImageData3);
	free(outputImageData4);
	free(outputImageData5);
	free(outputImageData6);

	return 0;
}
