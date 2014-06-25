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
#include "CImage.h"

class CInpainting{
	public:
	CInpainting();
	~CInpainting();
	void loadMask(char *maskPath);
	void displayMask();
	void detectMaskBorder();
	void patchSize(int x, int y);
	void computeConfidence(int i);
	void computeData(int i);
	void loadImg(char *imgPath, char *maskPath);
	int getMaxPrior();
	UList seekBestPatch(int n);
	bool isEmpty(int minX, int maxX, int minY, int maxY, Point point);
	bool isMaskEmpty();
	void inpaint(char *maskPath, char *imgPath);
	void updateAll(UList uList, int n);
	
	private :
	CMask mMask;
	Border mBorder;
	Point mHalfPatchSize; // This name is just horrible
	Point mSize;
	CImage mImg;
};

#endif