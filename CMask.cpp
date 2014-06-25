/*
 *  classMask.cpp
 *  inpaint
 *
 *  Created by LesKodaux on 10/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "CMask.h"

using namespace cimg_library;
using namespace std;

CMask::CMask(){
};

CMask::~CMask(){
};

void CMask::display(){
	CImgDisplay Disp(mMask);
	while (!Disp.is_closed()){
		Disp.wait();
	}
};

void CMask::load(char *maskPath){
	mMask.load(maskPath);
	mMask.threshold(128);
};

Point CMask::getSize(){
	Point maskSize;
	maskSize.x = mMask.width();
	maskSize.y = mMask.height();
	return maskSize;
};

Border CMask::detectMaskBorder(){
	Point flag;
	flag.x = 0;
	flag.y = 0;
	Border vect;
	vect.size = 0;
	vect.used = 0;
	// counting number of point in the mask to pre-allocate
	for (int i = 0; i < mMask.width(); i++)
		for (int j = 0; j < mMask.height(); j++)
			if (!mMask(i,j))
				vect.size++;
	vect.border.resize(vect.size);
	
	// scan following x
	for (int y = 0; y < mMask.height(); y++) {
		for (int x = 0; x < mMask.width(); x++) {
			if (flag.x == 0) {
				if (mMask(x,y) == 0) {
					flag.x = 1;
					vect.border[vect.used].Coord.x = x;
					vect.border[vect.used].Coord.y = y;
					vect.used++;
				}
			}
			else {
				if (mMask(x,y) == 1) {
					flag.x = 0;
					vect.border[vect.used].Coord.x = x-1;
					vect.border[vect.used].Coord.y = y;
					vect.used++;
				}
			}
		}
		flag.x = 0;
	}
	//scan following y
	for (int x = 0; x < mMask.width(); x++) {
		for (int y = 0; y < mMask.height(); y++) {
			if (flag.y == 0) {
				if (mMask(x,y) == 0) {
					flag.y = 1;
					vect.border[vect.used].Coord.x = x;
					vect.border[vect.used].Coord.y = y;
					vect.used++;
				}
			}
			else {
				if (mMask(x,y) == 1) {
					flag.y = 0;
					vect.border[vect.used].Coord.x = x;
					vect.border[vect.used].Coord.y = y-1;
					vect.used++;
				}
			}
		}
		flag.y = 0;
	}
	return vect;
};

void CMask::ChangeVal(Point point,RGB val){
	mMask(point.x,point.y,0,0) = val.r;
	mMask(point.x,point.y,0,1) = val.g;
	mMask(point.x,point.y,0,2) = val.b;
};

float CMask::getVal(Point point){
	return mMask(point.x,point.y,0,0);
};