#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "jpegio.h"

int main() {
	char *fileName = "testImage.jpg";
	BYTE *data = NULL;
	int imageByte = readJpeg(fileName, &data);
	int width = readHeader(fileName, IMAGE_WIDTH);
	int height = readHeader(fileName, IMAGE_HEIGHT);

	int redSum = 0, greenSum = 0, blueSum = 0;
	int x, y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			redSum += data[3 * (x + y * width)];
			greenSum += data[3 * (x + y * width) + 1];
			blueSum += data[3 * (x + y * width) + 2];		
		}
	}

	double redAverage = redSum / (double)(width * height);
	double greenAverage = greenSum / (double)(width * height);
	double blueAverage = blueSum / (double)(width * height);

	printf("average\n");
	printf("R:%0.1f, G:%0.1f, B:%0.1f\n", redAverage, greenAverage, blueAverage);

	redSum = 0;
	greenSum = 0;
	blueSum = 0;

	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			redSum += pow(data[3 * (x + y * width)] - redAverage, 2);
			greenSum += pow(data[3 * (x + y * width) + 1] - greenAverage, 2);
			blueSum += pow(data[3 * (x + y * width) + 2] -blueAverage, 2);
		}
	}

	double redDev = sqrt(redSum / (double)(width * height));	
	double greenDev = sqrt(greenSum / (double)(width * height));
	double blueDev = sqrt(blueSum / (double)(width * height));		

	printf("standard deviation\n");
	printf("R:%0.1f, G:%0.1f, B:%0.1f\n", redDev, greenDev, blueDev);

	return 0;
}
