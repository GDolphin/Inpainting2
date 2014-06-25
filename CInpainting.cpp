/*
 *  inapinting.cpp
 *  inpaint
 *
 *  Created by LesKodaux on 10/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "CInpainting.h"

using namespace cimg_library;
using namespace std;

CInpainting::CInpainting(){
	mHalfPatchSize.x = 3;
	mHalfPatchSize.y = 3;
};

CInpainting::~CInpainting(){
};

void CInpainting::loadMask(char *maskPath){
	mMask.load(maskPath);
	mSize = mMask.getSize();
};

void CInpainting::displayMask(){
	mMask.display();
};

void CInpainting::detectMaskBorder(){
	mBorder = mMask.detectMaskBorder();
};

void CInpainting::patchSize(int x, int y){ // modify the half patch size
	mHalfPatchSize.x = x;
	mHalfPatchSize.y = y;
};

void CInpainting::computeConfidence(int i){
	Point Coordinates;
	mBorder.border[i].confidence = 0;
	for (int j = -mHalfPatchSize.x; j < mHalfPatchSize.x + 1; j++) {
		if ((mBorder.border[i].Coord.x + j < mSize.x) and (mBorder.border[i].Coord.x + j >= 0)) {
			for (int k = -mHalfPatchSize.y; k < mHalfPatchSize.y + 1; k++) {
				if ((mBorder.border[i].Coord.y + k < mSize.y) and (mBorder.border[i].Coord.y + k >= 0)) {
					Coordinates.x = mBorder.border[i].Coord.x + j;
					Coordinates.y = mBorder.border[i].Coord.y + k;
					mBorder.border[i].confidence =  mBorder.border[i].confidence + mMask.getVal(Coordinates);
				}
			}
		}
	}
	mBorder.border[i].confidence = mBorder.border[i].confidence/((mHalfPatchSize.x * 2 + 1)*(mHalfPatchSize.y * 2 + 1));
};

void CInpainting::loadImg(char *imgPath, char *maskPath){
	mImg.loadImg(imgPath, maskPath);
};

void CInpainting::computeData(int i){
	FPoint normal;
	Point coord;
	FPoint grad, max;
	grad.x = 0;
	grad.y = 0;
	max.x = 0;
	max.y = 0;
	mBorder.border[i].data = 0;
	normal = mMask.getNormal(mBorder.border[i].Coord);
	for (int x = -mHalfPatchSize.x; x < mHalfPatchSize.x + 1; x++) {
		if ((mBorder.border[i].Coord.x + x < mSize.x) and (mBorder.border[i].Coord.x +x >=0))
			for (int y = -mHalfPatchSize.y; y < mHalfPatchSize.y + 1; y++) {
				if ((mBorder.border[i].Coord.y + y < mSize.y) and (mBorder.border[i].Coord.y +y >=0)) {
					coord.x = mBorder.border[i].Coord.x + x;
					coord.y = mBorder.border[i].Coord.y + y;
					grad = mImg.getGrad(coord);
					//cout << endl << grad.x << " " << grad.y;
					if ((grad.x*grad.x+ grad.y*grad.y) > (max.x*max.x+ max.y*max.y)) {
						max.x = grad.x;
						max.y = grad.y;
					}
				}
			}
	}
	mBorder.border[i].data = (normal.x * max.x + normal.y * max.y)/(sqrt((normal.x*normal.x+normal.y*normal.y)));
	if (mBorder.border[i].data < 0) // abs value
		mBorder.border[i].data = -mBorder.border[i].data;
};

int CInpainting::getMaxPrior(){
	int n = 0;
	float max = -1;
	for (int i = 0; i < mBorder.used; i++) {
		mBorder.border[i].priority = -1;
		computeData(i);
		computeConfidence(i);
		mBorder.border[i].priority =  mBorder.border[i].data * mBorder.border[i].confidence;
		if (mBorder.border[i].priority > max){
			n = i;
			max = mBorder.border[i].priority;
		}
	}
	return n;
};

UList CInpainting::seekBestPatch(int n){
	// Init
	int cSize = 0, x = mBorder.border[n].Coord.x, y = mBorder.border[n].Coord.y; //uSize : size of the updateList, cSize : size of the CoordList
	vector<Point> cList; // uList : list of points in the patch in the mask to update, cList : list of points in the patch out of mask
	vector<RGB> myPatch;
	float sizePatch = 0;
	RGB color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	Point coord;
	coord.x = 0;
	coord.y = 0;
	UList uList;
	uList.minX = mHalfPatchSize.x;
	uList.minY = mHalfPatchSize.y;
	uList.maxX = mHalfPatchSize.x;
	uList.maxY = mHalfPatchSize.y;
	uList.size = 0;
	
	
	// Adaptative patch size
	if(x < mHalfPatchSize.x)
		uList.minX = x;
	else if((mSize.x - x) < mHalfPatchSize.x)
		uList.maxX = mSize.x - x;
	if(y< mHalfPatchSize.y)
		uList.minY = y;
	else if((mSize.y - y) < mHalfPatchSize.y)
		uList.maxX = mSize.y - y;
	
	sizePatch = (uList.minX + uList.maxX + 1)*(uList.minY + uList.maxY + 1);
	// vector resize
	uList.list.resize(sizePatch);
	cList.resize(sizePatch);
	myPatch.resize(sizePatch);
	for (int i = x - uList.minX; i < x + uList.maxX + 1; i++)
		for (int j = y - uList.minY; j < y + uList.maxY + 1; j++) {
			coord.x = i;
			coord.y = j;
			if (mMask.getVal(coord)) { // out of mask
				myPatch[cSize] = mImg.getVal(coord);
				cList[cSize].x = i - x;
				cList[cSize].y = j - y;
				cSize++;
			}
			else {
				uList.list[uList.size].x = i - x;
				uList.list[uList.size].y = j - y;
				uList.size++;
			}

		}
	float initSSD = 600*600*sizePatch, SSD = initSSD, minSSD = initSSD;
	
	// looking for value to calculate the square difference and then sum = pixel out of mask
	for (int k = uList.minX; k < mSize.x - uList.maxX + 1; ++k)
		for (int l = uList.minY; l < mSize.y - uList.maxY + 1; ++l) {
			SSD = initSSD;
			coord.x = k;
			coord.y = l;
			if (isEmpty(uList.minX, uList.maxX, uList.minY, uList.maxY, coord)) {
				SSD = 0;
				for (int m = 0; m < cSize; ++m) {
					coord.x = k + cList[m].x;
					coord.y = l + cList[m].y;
					color = mImg.getVal(coord);
					color.r -= myPatch[m].r;
					color.g -= myPatch[m].g;
					color.b -= myPatch[m].b;
					SSD += color.r * color.r + color.g * color.g + color.b * color.b;
				}
			}
			if (SSD < minSSD) {
				minSSD = SSD;
				uList.coord.x = k;
				uList.coord.y = l;
			}
		}
	return uList;
};

bool CInpainting::isEmpty(int minX, int maxX, int minY, int maxY, Point point){
	// check that no point of the patch is in the mask
	Point coord;
	for (int i = -minX; i < maxX + 1; i ++)
		for (int j = - minY; j < maxY + 1; j++) {
			coord.x = point.x + i;
			coord.y = point.y + j;
			if (mMask.getVal(coord) == 0)
				return false;
		}
	return true;
};

bool CInpainting::isMaskEmpty(){
	// check that no point of the patch is in the mask
	Point coord;
	for (int i = 0; i < mSize.x; i ++)
		for (int j = 0; j < mSize.y + 1; j++) {
			coord.x = i;
			coord.y = j;
			if (mMask.getVal(coord) == 0)
				return false;
		}
	return true;
};

void CInpainting::inpaint(char *maskPath, char *imgPath){
	CImg<float> myMask;
	UList uList;
	int n = 0, i = 0;
	loadMask(maskPath);
	loadImg(imgPath,maskPath);
	patchSize(4, 4);
	while (!isMaskEmpty()) {
		detectMaskBorder();
		n = getMaxPrior();
		uList = seekBestPatch(n);
		updateAll(uList,n);
		if (i%50 == 0) {
			mImg.LabToRGB();
			mImg.displayImg();
			mImg.RGBToLab();
			mMask.display();
		}
		i++;
		//cout << i;
	}
	mImg.LabToRGB();
	mImg.displayImg();
	
};

void CInpainting::updateAll(UList uList, int n){
	RGB colorMask, colorImg;
	Point point;
	point.x = 0;
	point.y = 0;
	GradStruct colorGrad;
	colorMask.r = 1;
	colorMask.g = 1;
	colorMask.b = 1;
	colorImg.r = 0;
	colorImg.g = 0;
	colorImg.b = 0;
	for (int i = 0; i < uList.size; i++) {
		point.x = uList.list[i].x + uList.coord.x;
		point.y = uList.list[i].y + uList.coord.y;

		colorImg = mImg.getVal(point);
		colorGrad = mImg.getGradVal(point);
							   
		point.x = uList.list[i].x + mBorder.border[n].Coord.x;
		point.y = uList.list[i].y + mBorder.border[n].Coord.y;
							   
		mImg.changeVal(point, colorImg);
		mMask.changeVal(point, colorMask);
		mMask.computeNormal();
		mImg.changeGradVal(point, colorGrad);
	}
};