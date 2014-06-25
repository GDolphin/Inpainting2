/*
 *  inapinting.h
 *  inpaint
 *
 *  Created by LesKodaux on 10/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "classMask.h"
#include <vector>

class classInpainting{
	public:
	classInpainting();
	~classInpainting();
	void loadMask(char *maskPath);
	
	private :
	classMask mMask;
	std::vector<int> mBorder; // CREATE A CLASS AND A VECTOR OF THAT CLASS TO HAVE MULTIPLE VARIABLES
};