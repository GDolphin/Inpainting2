/*
 *  classMask.h
 *  inpaint
 *
 *  Created by LesKodaux on 10/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
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
	void ChangeVal(Point point,RGB val);
	float getVal(Point point);
	
	private :
	
	cimg_library::CImg<float> mMask;
};