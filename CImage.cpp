/*
 *  CImage.cpp
 *  inpaint
 *
 *  Created by LesKodaux on 12/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "CImage.h"

using namespace cimg_library;
using namespace std;

CImage::CImage(){
};

CImage::~CImage(){
};

void CImage::loadImg(char *imgPath, char *maskPath){
	CImg<float> erode;
	mImg.load(imgPath);
	RGBToLab();
	erode.load(maskPath);
	erode.threshold(128);
	erode.erode(3,3,1);
	computeGrad();
	
	for (int x = 0; x < mImg.width(); x++){
		for (int y = 0; y < mImg.height(); y++) {
			//cout << mImg(x,y,0,0) << " ";
			mGrad[0](x,y,0,0) = mGrad[0](x,y,0,0) * erode(x,y,0,0)/100;
			mGrad[0](x,y,0,1) = mGrad[0](x,y,0,1) * erode(x,y,0,0)/255;
			mGrad[0](x,y,0,2) = mGrad[0](x,y,0,2) * erode(x,y,0,0)/255;
			mGrad[1](x,y,0,0) = mGrad[1](x,y,0,0) * erode(x,y,0,0)/100;
			mGrad[1](x,y,0,1) = mGrad[1](x,y,0,1) * erode(x,y,0,0)/255;
			mGrad[1](x,y,0,2) = mGrad[1](x,y,0,2) * erode(x,y,0,0)/255;
		}
	//cout << endl;
	}
};


