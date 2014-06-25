/*
 *  inapinting.cpp
 *  inpaint
 *
 *  Created by LesKodaux on 10/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "inpainting.h"

using namespace cimg_library;

classInpainting::classInpainting(){
};

classInpainting::~classInpainting(){
};

void classInpainting::loadMask(char *maskPath){
	mMask.load(maskPath);
};
