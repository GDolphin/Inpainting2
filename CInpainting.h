/*
 *  inapinting.h
 *  inpaint
 *
 *  Created by LesKodaux on 10/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "CMask.h"
#include <vector>

class CInpainting{
	public:
	CInpainting();
	~CInpainting();
	void loadMask(char *maskPath);
	void displayMask();
	void detectMaskBorder();
	void patchSize(int x, int y);
	void computeConfidence();
	
	private :
	CMask mMask;
	Border mBorder;
	Point mHalfPatchSize; // This name is just horrible
	Point mSize;
};