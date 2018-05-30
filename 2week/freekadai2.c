#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jpegio.h"

//ycc変換
void ycc(int width, int height, BYTE* data) {
	int x, y;
	int y_, cb, cr;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			y_ = 0.257 * data[3 * (x + y * width)]
				+ 0.504 * data[3 * (x + y * width) + 1]
				+ 0.098 * data[3 * (x + y * width) + 2] + 16;
			cb = -0.148 * data[3 * (x + y * width)]
				- 0.291 * data[3 * (x + y * width) + 1]
				+ 0.439 * data[3 * (x + y * width) + 2] + 128;
			cr = 0.439 * data[3 * (x + y * width)]
				- 0.368 * data[3 * (x + y * width) + 1]
				- 0.0718 * data[3 * (x + y * width)] + 128;

			data[3 * (x + y * width)] = y_;
			data[3 * (x + y * width) + 1] = cb;
			data[3 * (x + y * width) + 2] = cr;
		}
	}		
}

//0red,1green,2blue
double getAverage(int width, int height, BYTE* data, int color, int i) {
	int sum = 0;
	int x, y;
	if (color < 0 || color > 2) return 0;
	for (x = 0; x < width; x++) {
		for (y = i * height; y < i * height + height ; y++) {
			sum += data[3 * (x + y * width) + color];	
		}
	}

	return sum / (double)(width * height);
}

int main() {
	char *fileName1 = "whiteJinbutu.jpg";
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

	BYTE *outputData = NULL;
	outputData = (BYTE*)malloc(jinbutuImageByte);

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

	BYTE *jinbutuCopyData = (BYTE*)malloc(jinbutuImageByte);
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			jinbutuCopyData[3 * (x + y * width)] = jinbutuData[3 * (x + y * width)];
			jinbutuCopyData[3 * (x + y * width) + 1] = jinbutuData[3 * (x + y * width)+1];
			jinbutuCopyData[3 * (x + y * width) + 2] = jinbutuData[3 * (x + y * width)+2];
		}
	}

	ycc(width, height, kuromakiData);
	ycc(width, height, jinbutuCopyData);

	int d;
	printf("d -> ");
	scanf("%d", &d);

	int devide = 2;
	height /= devide;
	int i;	
	for (i = 0; i < devide; i++) {
		//クロマキー画像の平均値取得
		double redAverage = getAverage(width, height / devide, kuromakiData, 0, i);
		double greenAverage = getAverage(width, height / devide, kuromakiData, 1, i);
		double blueAverage = getAverage(width, height / devide, kuromakiData, 2, i);
	

		//青色判別とmaskの更新
		for (x = 0; x < width; x++) {
			for (y = i * height; y < i * height + height; y++) {
				if (jinbutuCopyData[3 * (x + y * width)] + jinbutuCopyData[3 * (x + y * width) + 1] + jinbutuCopyData[3 * (x + y * width) + 2] > 320) {
				if ((double)abs((double)jinbutuCopyData[3 * (x + y * width)] - redAverage) < d * 3
				    && (double)abs((double)jinbutuCopyData[3 * (x + y * width) + 1] - greenAverage) < d * 3
				    && (double)abs((double)jinbutuCopyData[3 * (x + y * width) + 2] - blueAverage) < d * 0.8 ) {
					mask[3 * (x + y * width)] = 0;
					mask[3 * (x + y * width) + 1] = 0;
					mask[3 * (x + y * width) + 2] = 0;
				}
				}
			}
		}

		//背景画像と人物画像合成
		for (x = 0; x < width; x++) {
			for (y = i * height; y < i * height + height; y++) {
				if (mask[3 * (x + y * width)] == 0) {
					outputData[3 * (x + y * width)] = haikeiData[3 * (x + y * width)];
					outputData[3 * (x + y * width)+1] = haikeiData[3 * (x + y * width)+1];
					outputData[3 * (x + y * width)+2] = haikeiData[3 * (x + y * width)+2];
				} else {
					outputData[3 * (x + y * width)] = jinbutuData[3 * (x + y * width)];
					outputData[3 * (x + y * width)+1] = jinbutuData[3 * (x + y * width)+1];
					outputData[3 * (x + y * width)+2] = jinbutuData[3 * (x + y * width)+2];
				}						 
			}
		}
	}

	

	//出力
	if (writeJpeg("resultfreemask.jpg", mask, width, height * devide, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	if (writeJpeg("resultfreekadai2.jpg", outputData, width, height * devide, bpp, 100) == 1) {
		printf("success\n");
	} else {
		printf("not success\n");
	}

	free(mask);

	return 0;
}
