/*
 *  inapinting.h
 *  inpaint
 *
 *  Created by LesKodaux on 10/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CINPAINTING_HEADER
#define CINPAINTING_HEADER
#include "CMask.h"

class CInpainting{
	public:
	CInpainting(char *maskPath, char *imgPath,int halfWinX, int halfWinY);
	~CInpainting();
	void detectMaskBorder();
	void patchSize(int x, int y);
	void computeConfidence(int i);
	void computeData(int i);
	int getMaxPrior();
	UList seekBestPatch(int n);
	bool isEmpty(int minX, int maxX, int minY, int maxY, Point point);
	bool isMaskEmpty();
	void inpaint();
	void updateAll(UList uList, int n);
	void computeGrad();
	
	private :
	Border mBorder;
	Point mHalfPatchSize; // This name is just horrible
	Point mSize;
	CMask mMask;
	cimg_library::CImg<float> mImg;
	std::vector<cimg_library::CImg<float> > mGrad;
	float *imgL,*imgA,*imgB,*gradX,*gradY,*gradX2, *gradY2, *mask2;
};

#endif