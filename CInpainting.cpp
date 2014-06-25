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

CInpainting::CInpainting(char *maskPath, char *imgPath,int halfWinX, int halfWinY){
	int area = 0,  position = 0;
	Point coord;
	coord.x = 0;
	coord.y = 0;
	CImg<float> eMask; // for erosion
	
	mHalfPatchSize.x = halfWinX;
	mHalfPatchSize.y = halfWinY;
	
	// load images + mask
	mImg.load(imgPath);
	mMask.load(maskPath);
	eMask.load(maskPath);
	eMask.threshold(128);
	eMask.erode(3,3,1);
	mImg.sRGBtoRGB();
	mImg.RGBtoLab();
	mSize.x = mImg.width();
	mSize.y = mImg.height();
	
	area = mSize.x * mSize.y;
	
	imgL = new float[area];
	imgA = new float[area];
	imgB = new float[area];
	gradX = new float[area];
	gradY = new float[area];
	gradX2 = new float[area];
	gradY2 = new float[area];
	mask2 = new float[area];

	// compute gradient
	computeGrad();
	// Copy IMG into a vector
	
	for (int x = 0; x < mSize.x; ++x){
		coord.x = x;
		for (int y = 0; y < mSize.y; ++y) {
			coord.y = y;
			position = x + y * mSize.x;
			imgL[position] = mImg(x,y,0,0);
			imgA[position] = mImg(x,y,0,1);
			imgB[position] = mImg(x,y,0,2);
			gradX2[position] = (mGrad[0](x,y,0,0)*mGrad[0](x,y,0,0)+mGrad[0](x,y,0,1)*mGrad[0](x,y,0,1)+mGrad[0](x,y,0,2)*mGrad[0](x,y,0,2))*eMask(x,y,0,0);
			gradX[position] = sqrt(gradX2[position]);
			gradY2[position] = (mGrad[1](x,y,0,0)*mGrad[0](x,y,0,0)+mGrad[1](x,y,0,1)*mGrad[0](x,y,0,1)+mGrad[1](x,y,0,2)*mGrad[0](x,y,0,2))*eMask(x,y,0,0);
			gradY[position] = sqrt(gradY2[position]);
			mask2[position] = mMask.getVal(coord);
		}
	}
};

CInpainting::~CInpainting(){
	delete[] imgL;
	delete[] imgA;
	delete[] imgB;
	delete[] gradX;
	delete[] gradY;
	delete[] gradX2;
	delete[] gradY2;
};

void CInpainting::computeGrad(){
	vector<CImg<float> > var(2);
	CImg<float> img;
	img = mImg;
	var[0] = img;
	var[1] = img;
	
	for (int x =0; x < img.width(); x++) {
		for (int y = 0; y < img.height(); y++) {
			for (int k = 0; k < 3 ; k++) {
				//img(x,y,0,k) = int(img(x,y,0,k));
				var[0](x,y,0,k) = 0;
				var[1](x,y,0,k) = 0;
			}
		}
	}
	img.blur(3,3,1,false,false);
	
	for (int x = 1; x < img.width()-1; x++){
		for (int j = 0; j < 3; j++){
			var[0](x,0,0,j) = (img(x+1,0,0,j) - img(x-1,0,0,j))/2;
			var[0](x,img.height()-1,0,j) = (img(x+1,img.height()-1,0,j) - img(x-1,img.height()-1,0,j))/2;
			var[1](x,0,0,j) = (img(x,1,0,j) - img(x,0,0,j))/2;
			var[1](x,img.height()-1,0,j) = (img(x,img.height()-2,0,j) - img(x-1,img.height()-1,0,j))/2;
		}
		for (int y = 1; y < img.height()-1; y++)
			for (int i = 0; i < 2; i++)
				for (int j =0; j < 3; j++)
					var[i](x,y,0,j) = (img(x+1-i,y+i,0,j) - img(x-1+i,y-i,0,j))/2;
	}
	for (int y = 1; y < img.height()-1; y++)
		for (int j = 0; j < 3; j++){
			var[0](0,y,0,j) = (img(0,y,0,j) - img(1,y,0,j))/2;
			var[0](img.width()-1,0,j) = (img(img.width()-2,y,0,j) - img(img.width()-1,y,0,j))/2;
			var[1](0,y,0,j) = (img(0,y+1,0,j) - img(0,y-1,0,j))/2;
			var[1](img.width()-1,0,j) = (img(img.width()-1,y+1,0,j) - img(img.width()-1,y-1,0,j))/2;
		}
	for (int j = 0; j < 3; j++) {
		var[0](0,0,0,j) = img(1,0,0,j) - img(0,0,0,j);
		var[0](0,img.height()-1,j) =  img(1,img.height()-1,0,j) - img(0,img.height()-1,0,j);
		var[0](img.width()-1,0,0,j) = img(img.width()-2,0,0,j) - img(img.width()-1,0,0,j);
		var[0](img.height()-1,j) =  img(img.width()-2,img.height()-1,0,j) - img(img.width()-1,img.height()-1,0,j);
		
		var[1](0,0,0,j) = img(0,1,0,j) - img(0,0,0,j);
		var[1](0,img.height()-1,j) =  img(0,img.height()-2,0,j) - img(0,img.height()-1,0,j);
		var[1](img.width()-1,0,0,j) = img(img.width()-1,1,0,j) - img(img.width()-1,0,0,j);
		var[1](img.height()-1,j) =  img(img.width()-1,img.height()-2,0,j) - img(img.width()-1,img.height()-1,0,j);
	}
	mGrad = var;
};



void CInpainting::detectMaskBorder(){
	mBorder = mMask.detectMaskBorder();
};


void CInpainting::computeConfidence(int i){
	int x = 0, y = 0;
	mBorder.border[i].confidence = 0;
	for (int j = -mHalfPatchSize.x; j < mHalfPatchSize.x + 1; ++j) {
		x = mBorder.border[i].Coord.x + j;
		if ((x < mSize.x) and (x >= 0)) {
			for (int k = -mHalfPatchSize.y; k < mHalfPatchSize.y + 1; ++k) {
				y = mBorder.border[i].Coord.y + k;
				if ((y < mSize.y) and (y >= 0)) {
					mBorder.border[i].confidence =  mBorder.border[i].confidence + mask2[x + y * mSize.x];
				}
			}
		}
	}
	mBorder.border[i].confidence = mBorder.border[i].confidence/((mHalfPatchSize.x * 2 + 1)*(mHalfPatchSize.y * 2 + 1));
};

void CInpainting::computeData(int i){
	FPoint normal, max;
	int posX = 0, posY = 0, position = 0;
	max.x = 0;
	max.y = 0;
	mBorder.border[i].data = 0;
	normal = mMask.getNormal(mBorder.border[i].Coord);
	for (int x = -mHalfPatchSize.x; x < mHalfPatchSize.x + 1; ++x) {
		posX = mBorder.border[i].Coord.y + x;
		if ((posX < mSize.x) and (posX >=0))
			for (int y = -mHalfPatchSize.y; y < mHalfPatchSize.y + 1; ++y) {
				posY = mBorder.border[i].Coord.y + y;
				if ((posY < mSize.y) and (posY >=0)) {
					position = posX + posY * mSize.x;
					if ((gradX2[position]+ gradY2[position]) > (max.x*max.x+ max.y*max.y)) {
						max.x = gradX[position];
						max.y = gradY[position];
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
	int cSize = 0, x = mBorder.border[n].Coord.x, y = mBorder.border[n].Coord.y, posX = 0, posY = 0, position = 0; //uSize : size of the updateList, cSize : size of the CoordList
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
	
	for (int i = x - uList.minX; i < x + uList.maxX + 1; ++i)
		for (int j = y - uList.minY; j < y + uList.maxY + 1; ++j) {
			position = i + j * mSize.x;
			if (mask2[position]) { // out of mask
				myPatch[cSize].r = imgL[position];
				myPatch[cSize].g = imgA[position];
				myPatch[cSize].b = imgB[position];
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
	for (int k = uList.minX; k < mSize.x - uList.maxX; ++k)
		for (int l = uList.minY; l < mSize.y - uList.maxY; ++l) {
			SSD = initSSD;
			coord.x = k;
			coord.y = l;
			if (isEmpty(uList.minX, uList.maxX, uList.minY, uList.maxY, coord)) {
				SSD = 0;
				for (int m = 0; m < cSize; ++m) {
					posX = k + cList[m].x;
					posY = l + cList[m].y;
					position = posX + posY * mSize.x;
					color.r = imgL[position] - myPatch[m].r;
					color.g = imgA[position] - myPatch[m].g;
					color.b = imgB[position] - myPatch[m].b;
					SSD += color.r*color.r + color.g*color.g+color.b*color.b;
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
			if (mask2[coord.x + coord.y * mSize.x] == 0)
				return false;
		}
	return true;
};

bool CInpainting::isMaskEmpty(){
	// check that the mask is not empty
	Point coord;
	for (int i = 0; i < mSize.x; i ++)
		for (int j = 0; j < mSize.y + 1; j++) {
			coord.x = i;
			coord.y = j;
			if (mask2[coord.x + coord.y * mSize.x] == 0)
				return false;
		}
	return true;
};

void CInpainting::inpaint(){
	UList uList;
	int n = 0, i = 0;
	while (!isMaskEmpty()) {
		detectMaskBorder();
		n = getMaxPrior();
		uList = seekBestPatch(n);
		updateAll(uList,n);
		if (i%10 == 0) {
			mMask.display();	
		}
		i++;
		//cout << i;
	}
	//
	//mImg.LabToRGB();
	//mImg.displayImg();
	
};

void CInpainting::updateAll(UList uList, int n){ // A MODIFIER
	RGB colorMask, colorImg;
	int position = 0, positionRep = 0;
	Point point, pointRep;
	point.x = 0;
	point.y = 0;
	pointRep.x = 0;
	pointRep.y = 0;
	colorMask.r = 1;
	colorMask.g = 1;
	colorMask.b = 1;
	colorImg.r = 0;
	colorImg.g = 0;
	colorImg.b = 0;
	for (int i = 0; i < uList.size; i++) {
		point.x = uList.list[i].x + uList.coord.x;
		point.y = uList.list[i].y + uList.coord.y;
		position = point.x + point.y * mSize.x;
		pointRep.x = uList.list[i].x + mBorder.border[n].Coord.x;
		pointRep.y = uList.list[i].y + mBorder.border[n].Coord.y;
		positionRep = pointRep.x + pointRep.y * mSize.x;
		
		imgL[positionRep] = imgL[position];
		imgA[positionRep] = imgA[position];
		imgB[positionRep] = imgB[position];
		gradX[positionRep] = gradX[position];
		gradX2[positionRep] = gradX2[position];
		gradY[positionRep] = gradY[position];
		gradY2[positionRep] = gradY2[position];
		mask2[positionRep] = 1;
		mMask.changeVal(pointRep, colorMask);
	}
	mMask.computeNormal();
};