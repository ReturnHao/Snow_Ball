//
//  Texture.cpp
//  Snow_Ball_Game
//
//  Created by Return on 2018/12/30.
//  Copyright © 2018 Return. All rights reserved.
//
#define _CRT_SECURE_NO_WARNINGS    
#include <stdio.h>    
#include <windows.h>
#include "Texture.hpp"    
#define BITMAP_ID 0x4D42     
      
static unsigned char *LoadBitmapFile(const char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;          
	BITMAPFILEHEADER bitmapFileHeader;  
	unsigned char *bitmapImage;     
	int imageIdx = 0;       
	unsigned char tempRGB;     

	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) {
		printf("file not open\n");
		return NULL;
	}

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
  
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}    
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);     
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];

	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}
	
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	
	fclose(filePtr);
	return bitmapImage;
}

static unsigned char *LoadBitmapFile(const char *filename, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char* backgroundColor)
{

	FILE *filePtr;   
	BITMAPFILEHEADER bitmapFileHeader;     
	unsigned char *bitmapImage;         
	int imageIdx = 0;       

	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) {
		printf("file not open\n");
		return NULL;
	}
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}
	
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}
	unsigned char*   bitmapData;   

	bitmapData = new unsigned char[bitmapInfoHeader->biSizeImage / 3 * 4];

	int count = 0;
	
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		bitmapData[count] = bitmapImage[imageIdx + 2];
		bitmapData[count + 1] = bitmapImage[imageIdx + 1];
		bitmapData[count + 2] = bitmapImage[imageIdx];
		if (bitmapData[count] >= backgroundColor[0]
			&& bitmapData[count + 1] >= backgroundColor[1]
			&& bitmapData[count + 2] >= backgroundColor[2]) {
			bitmapData[count + 3] = 0;
		}
		else bitmapData[count + 3] = 255;
		count += 4;
	}

	
	fclose(filePtr);
	return bitmapData;
}

void loadTex(int i, const char *filename, GLuint* texture)
{

	BITMAPINFOHEADER bitmapInfoHeader;       
	unsigned char*   bitmapData;               

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);

	glBindTexture(GL_TEXTURE_2D, texture[i]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0,           
		GL_RGB,    
		bitmapInfoHeader.biWidth, 
		bitmapInfoHeader.biHeight,       
		0,       
		GL_RGB,     
		GL_UNSIGNED_BYTE, 
		bitmapData);    

}

void loadTex(int i, const char *filename, GLuint* texture, unsigned char* backgroundColor)
{

	BITMAPINFOHEADER bitmapInfoHeader;      
	unsigned char*   bitmapData;       

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader, backgroundColor);

	glBindTexture(GL_TEXTURE_2D, texture[i]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0,         
		GL_RGBA,   
		bitmapInfoHeader.biWidth, 
		bitmapInfoHeader.biHeight, 
		0,
		GL_RGBA, 
		GL_UNSIGNED_BYTE, 
		bitmapData);    
}