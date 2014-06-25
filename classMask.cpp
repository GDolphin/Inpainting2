/*
 *  classMask.cpp
 *  inpaint
 *
 *  Created by LesKodaux on 10/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "classMask.h"

using namespace cimg_library;
using namespace std;

classMask::classMask(){
};

classMask::~classMask(){
};

void classMask::display(){
	CImgDisplay Disp(mMask);
	while (!Disp.is_closed()){
		Disp.wait();
	}
};

void classMask::load(char *maskPath){
	mMask.load(maskPath);
	mMask.threshold(128);
};