/*
 *  CImage.h
 *  inpaint
 *
 *  Created by LesKodaux on 12/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef CIMAGE_HEADER
#define CIMAGE_HEADER
#include "CImg.h"
#include "MyStruct.h"
#include <iostream>
#include <cmath>

class CImage{
	public:
	CImage();
	~CImage();
	void loadImg(char *imgPath, char *maskPath);
	void computeGrad();
	void displayImg();
	RGB getVal(Point point);
	GradStruct getGradVal(Point point);
	void changeVal(Point point,RGB val);
	void changeGradVal(Point point, GradStruct val);
	void LabToRGB();
	void RGBToLab();
	FPoint getGrad(Point point);
	void RGBToXYZ();
	void XYZToRGB();
	void XYZToLab();
	void LabToXYZ();
	float f(float n);
	float fInv(float n);
	
	private:
	cimg_library::CImg<float> mImg;
	std::vector<cimg_library::CImg<float> > mGrad;
};
#endif