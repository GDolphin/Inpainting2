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

void CInpainting::computeConfidence(){
	Point Coordinates;
	for (int i = 0; i < mBorder.used ; i++) {
		for (int j = -mHalfPatchSize.x; j < mHalfPatchSize.x + 1; j++) {
			if ((mBorder.border[i].Coord.x + j < mSize.x) and (mBorder.border[i].Coord.x + j >= 0)) {
				for (int k = -mHalfPatchSize.y; k < mHalfPatchSize.y + 1; k++) {
					if ((mBorder.border[i].Coord.y + k < mSize.y) and (mBorder.border[i].Coord.y + k >= 0)) {
						Coordinates.x = mBorder.border[i].Coord.x + j;
						Coordinates.y = mBorder.border[i].Coord.y + k;
						mBorder.border[i].confidence =  mBorder.border[i].confidence + !mMask.getVal(Coordinates);
					}
				}
			}
		}
	}
};


