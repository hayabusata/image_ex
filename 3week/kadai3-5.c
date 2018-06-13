#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jpegio.h"

double func(double t) {
	if (t > 0.008856) {
		return pow(t, 1.0 / 3.0);
	} else {
		return 7.7871 * t + (16.0 / 116.0);
	}
}

BYTE* getCIELAB(BYTE* data, int width, int height) {
	int x, y;
	double* lrgb = (double*)malloc(width * height * 3 * sizeof(double) / sizeof(BYTE));
	double* ciexyz = (double*)malloc(width * height * 3 * sizeof(double) / sizeof(BYTE));
	double* cielab = (double*)malloc(width * height * 3 * sizeof(double) / sizeof(BYTE));

	//リニアRGBの取得
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			lrgb[3 * (x + y * width)] = pow(data[3 * (x + y * width)] / 255.0, 2.2);
			lrgb[3 * (x + y * width) + 1] = pow(data[3 * (x + y * width) + 1] / 255.0, 2.2);
			lrgb[3 * (x + y * width) + 2] = pow(data[3 * (x + y * width) + 2] / 255.0, 2.2);
		}	
	}

	//CIEXYZ
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			ciexyz[3 * (x + y * width)] = 100 * (
							0.4124 * lrgb[3 * (x + y * width)]
							+ 0.3576 * lrgb[3 * (x + y * width)+1]
							+ 0.1805 * lrgb[3 * (x + y * width)+2]);
			ciexyz[3 * (x + y * width)+1] = 100 * (
							0.2126 * lrgb[3 * (x + y * width)]
							+ 0.7152 * lrgb[3 * (x + y * width)+1]
							+ 0.0722 * lrgb[3 * (x + y * width)+2]);
			ciexyz[3 * (x + y * width)+2] = 100 * (
							0.0193 * lrgb[3 * (x + y * width)]
							+ 0.1192 * lrgb[3 * (x + y * width)+1]
							+ 0.9504 * lrgb[3 * (x + y * width)+2]);	
		}
	}

	//CIELAB
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			//L*
			cielab[3 * (x + y * width)] = 116 * func(ciexyz[3 * (x + y * width)+1] / 100);
			//a*			
			cielab[3 * (x + y * width)+1] = 500*(func(ciexyz[3 * (x + y * width)] / 95.05)
							- func(ciexyz[3 * (x + y * width)+1] / 100));
			//b*
			cielab[3 * (x + y * width)+2] = 200*(func(ciexyz[3 * (x + y * width)+1] / 100)
							- func(ciexyz[3 * (x + y * width)+2] / 108.89));
		}
	}

	free(lrgb);
	free(ciexyz);

	return cielab;
}

double getEab(double* cielab1, double* cielab2, int width, int height) {
	int x, y;
	double sum = 0;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			sum += sqrt(pow(cielab1[3*(x + y * width)] - cielab2[3*(x + y * width)], 2)
				+ pow(cielab1[3*(x + y * width)+1] - cielab2[3*(x + y * width)+1], 2)
				+ pow(cielab1[3*(x + y * width)+2] - cielab2[3*(x + y * width)+2], 2));
		}
	}
	
	return sum / (width * height);
}

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

	double *cielab1 = getCIELAB(inputImageData1, width, height);
	double *cielab2 = getCIELAB(inputImageData2, width, height);

	

	printf("Eab = %f\n", getEab(cielab1, cielab2, width, height));

	free(inputImageData1);
	free(inputImageData2);
	free(cielab1);
	free(cielab2);

	return 0;
}
