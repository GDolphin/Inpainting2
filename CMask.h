/*
 *  classMask.h
 *  inpaint
 *
 *  Created by LesKodaux on 10/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMASK_HEADER
#define CMASK_HEADER
#include "CImg.h"
#include <iostream>
#include "MyStruct.h"


class CMask{
	public :
	
	CMask();
	~CMask();
	void display();
	void load(char *maskPath);
	Point getSize();
	Border detectMaskBorder();
	int countPoint();
	void changeVal(Point point,RGB val);
	float getVal(Point point);
	FPoint getNormal(Point point);
	void computeNormal();
	
	private :
	
	cimg_library::CImg<float> mMask;
	std::vector<cimg_library::CImg<float> > mNormal;
};

#endif