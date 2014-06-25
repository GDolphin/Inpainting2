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


class classMask{
	public :
	
	classMask();
	~classMask();
	void display();
	void load(char *maskPath);
	
	private :
	
	cimg_library::CImg<float> mMask;
};