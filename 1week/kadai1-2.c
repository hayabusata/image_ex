#include<stdio.h>
#include<stdlib.h>
#include "jpegio.h"

int main() {
	char *fileName = "testImage.jpg";
	BYTE *data = NULL;
	int imageByte = readJpeg(fileName, &data);
	int width = readHeader(fileName, IMAGE_WIDTH);
	int height = readHeader(fileName, IMAGE_HEIGHT);

	int x = 0, y = 0;
	do {
		printf("x座標 -> ");
		scanf("%d", &x);
		if (x > width) printf("over x\n");
	} while (x > width);
	do {
		printf("y座標 -> ");
		scanf("%d", &y);
		if (y > height) printf("over y\n");
	} while (y > height);

	printf("%d, %d, %d", data[3 * (x + y * width)], data[3 * (x + y * width) + 1], data[3 * (x + y * width) + 2]);

	return 0;
}
