#pragma once
#include "Config.h"

//struct Image {
//	unsigned long sizeX;
//	unsigned long sizeY;
//	char* data;
//};
//
//int ImageLoad(const char* filename, Image* image) {
//
//	FILE* file;
//	unsigned long size; // size of the image in bytes.
//	unsigned long i; // standard counter.
//	unsigned short int planes; // number of planes in image (must be 1)
//	unsigned short int bpp; // number of bits per pixel (must be 24)
//	char temp; // temporary color storage for bgr-rgb conversion.
//
//	fopen_s(&file, filename, "rb");
//
//	// make sure the file is there.
//	if (file == NULL) {
//		printf("File Not Found : %s\n", filename);
//		return 0;
//	}
//
//	// seek through the bmp header, up to the width/height:
//	fseek(file, 18, SEEK_CUR);
//
//	// read the width
//	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
//		printf("Error reading width from %s.\n", filename);
//		return 0;
//	}
//
//	// read the height
//	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
//		printf("Error reading height from %s.\n", filename);
//		return 0;
//	}
//
//	// calculate the size (assuming 24 bits or 3 bytes per pixel).
//	size = image->sizeX * image->sizeY * 3;
//
//	// read the planes
//	if ((fread(&planes, 2, 1, file)) != 1) {
//		printf("Error reading planes from %s.\n", filename);
//		return 0;
//	}
//	if (planes != 1) {
//		printf("Planes from %s is not 1: %u\n", filename, planes);
//		return 0;
//	}
//	// read the bitsperpixel
//	if ((i = fread(&bpp, 2, 1, file)) != 1) {
//		printf("Error reading bpp from %s.\n", filename);
//		return 0;
//	}
//	if (bpp != 24) {
//		printf("Bpp from %s is not 24: %u\n", filename, bpp);
//		return 0;
//	}
//	// seek past the rest of the bitmap header.
//	fseek(file, 24, SEEK_CUR);
//
//	// read the data.
//	image->data = (char*)malloc(size);
//
//	if (image->data == NULL) {
//		printf("Error allocating memory for color-corrected image data");
//		return 0;
//	}
//	if ((i = fread(image->data, size, 1, file)) != 1) {
//		printf("Error reading image data from %s.\n", filename);
//		return 0;
//	}
//	for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)
//		temp = image->data[i];
//		image->data[i] = image->data[i + 2];
//		image->data[i + 2] = temp;
//	}
//	// we're done.
//	return 1;
//}
//
//Image* loadTexture(const char* filename) {
//	Image* result;
//	// allocate space for texture
//	result = (Image*)malloc(sizeof(Image));
//
//	if (result == NULL) {
//		std::cout << "Error allocating space for image" << std::endl;
//		exit(0);
//	}
//
//	if (!ImageLoad(filename, result)) {
//		exit(1);
//	}
//
//	// final check after image has been loaded.
//	if (result == NULL) {
//		std::cout << "Image was not returned from loadTexture. File: " << filename << std::endl;
//		exit(0);
//	}
//
//	return result;
//}
//
//void parseTexture(Image* img, int id) {
//
//	// Create Texture
//	glGenTextures(1, &textureCollection.at(id));
//	glBindTexture(GL_TEXTURE_2D, textureCollection.at(id));
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
//	glTexImage2D(GL_TEXTURE_2D, 0, 3, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//
//}
//







// returning 1: animation is finished.
// returning 0: it's being played
// returning -1: doesn't exist or called yet 
// returning a int value if frames is available
int animateTex(int id, int delay = 0, bool frames = false) {

	// if there's no frames then just switch to another texture after a delay.
	// check if there's already a delay. in which case update it.
	if (globalAnimationDelay.count(id) != 0) {

		int value = globalAnimationDelay.at(id) - ANIMATIONSTEP;

		if (value <= 0) {
			globalAnimationDelay.erase(id);
			return 1;
		}
		globalAnimationDelay.insert_or_assign(id, value);
		if (frames) return value;
		
	}
	else if (globalAnimationDelay.count(id) == 0 && delay == 0) {
		return -1;
	} else globalAnimationDelay.insert_or_assign(id, delay);

	return 0;

}