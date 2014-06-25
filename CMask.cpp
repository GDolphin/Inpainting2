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
	mNormal.resize(2);
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
	computeNormal();
};

Point CMask::getSize(){
	Point maskSize;
	maskSize.x = mMask.width();
	cout << maskSize.x;
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
			if (mMask(i,j,0,0) < 0.5)
				vect.size++;
	vect.size = vect.size * 2;
	vect.border.resize(vect.size);
	
	// scan following x
	for (int y = 0; y < mMask.height(); y++) {
		for (int x = 0; x < mMask.width(); x++) {
			vect.border[vect.used].data = 0;
			vect.border[vect.used].confidence = 0;
			vect.border[vect.used].priority = 0;
			if (flag.x == 0) {
				if (mMask(x,y,0,0) < 0.5) {
					flag.x = 1;
					vect.border[vect.used].Coord.x = x;
					vect.border[vect.used].Coord.y = y;
					vect.used++;
				}
			}
			else {
				if (mMask(x,y,0,0) > 0.5) {
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
	for (int x = 0; x < mMask.width(); ++x) {
		for (int y = 0; y < mMask.height(); ++y) {
			vect.border[vect.used].data = 0; // Y a un bug ici, après beaucoup d'itérations
			vect.border[vect.used].confidence = 0;
			vect.border[vect.used].priority = 0;
			if (flag.y == 0) {
				if (mMask(x,y,0,0) < 0.5) {
					flag.y = 1;
					vect.border[vect.used].Coord.x = x;
					vect.border[vect.used].Coord.y = y;
					vect.used++;
				}
			}
			else {
				if (mMask(x,y,0,0) > 0.5) {
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

void CMask::changeVal(Point point,RGB val){
	mMask(point.x,point.y,0,0) = val.r;
	mMask(point.x,point.y,0,1) = val.g;
	mMask(point.x,point.y,0,2) = val.b;
};

float CMask::getVal(Point point){
	float a = mMask(point.x,point.y,0,0);
	return a;
};

FPoint CMask::getNormal(Point point){
	FPoint normal;
	normal.x = mNormal[0](point.x,point.y,0,0);
	normal.y = mNormal[1](point.x,point.y,0,0);
	return normal;
};

void CMask::computeNormal(){
	vector<CImg<float> > var(2);
	CImg<float> temp = mMask;
	mMask.blur(3,3,true,true);
	var[0] = mMask;
	var[1] = mMask;
	for (int x = 1; x < mMask.width()-1; x++){
			var[0](x,0,0,0) = mMask(x+1,0,0,0) - mMask(x-1,0,0,0);
			var[0](x,mMask.height()-1,0,0) = mMask(x+1,mMask.height()-1,0,0) - mMask(x-1,mMask.height()-1,0,0);
			var[1](x,0,0,0) = mMask(x,1,0,0) - mMask(x,0,0,0);
			var[1](x,mMask.height()-1,0,0) = mMask(x,mMask.height()-2,0,0) - mMask(x-1,mMask.height()-1,0,0);
		for (int y = 1; y < mMask.height()-1; y++)
			for (int i = 0; i < 2; i++)
				var[i](x,y,0,0) = mMask(x+1-i,y+i,0,0) - mMask(x-1+i,y-i,0,0);
	}
	for (int y = 1; y < mMask.height()-1; y++){
		var[0](0,y,0,0) = mMask(0,y,0,0) - mMask(1,y,0,0);
		var[0](mMask.width()-1,0,0) = mMask(mMask.width()-2,y,0,0) - mMask(mMask.width()-1,y,0,0);
		var[1](0,y,0,0) = mMask(0,y+1,0,0) - mMask(0,y-1,0,0);
		var[1](mMask.width()-1,0,0) = mMask(mMask.width()-1,y+1,0,0) - mMask(mMask.width()-1,y-1,0,0);
	}
	var[0](0,0,0,0) = mMask(1,0,0,0) - mMask(0,0,0,0);
	var[0](0,mMask.height()-1,0) =  mMask(1,mMask.height()-1,0,0) - mMask(0,mMask.height()-1,0,0);
	var[0](mMask.width()-1,0,0,0) = mMask(mMask.width()-2,0,0,0) - mMask(mMask.width()-1,0,0,0);
	var[0](mMask.height()-1,0) =  mMask(mMask.width()-2,mMask.height()-1,0,0) - mMask(mMask.width()-1,mMask.height()-1,0,0);
	
	var[1](0,0,0,0) = mMask(0,1,0,0) - mMask(0,0,0,0);
	var[1](0,mMask.height()-1,0) =  mMask(0,mMask.height()-2,0,0) - mMask(0,mMask.height()-1,0,0);
	var[1](mMask.width()-1,0,0,0) = mMask(mMask.width()-1,1,0,0) - mMask(mMask.width()-1,0,0,0);
	var[1](mMask.height()-1,0) =  mMask(mMask.width()-1,mMask.height()-2,0,0) - mMask(mMask.width()-1,mMask.height()-1,0,0);
		mNormal = var;
	mMask = temp;
	
	
};
