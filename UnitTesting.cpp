/*
 *  UnitTesting.cpp
 *  inpaint
 *
 *  Created by LesKodaux on 12/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
/*
#include "UnitTesting.h"
using namespace std;

int unitTest(){
	char *maskPath = "ship-mask.png", *imgPath = "ship-input.png"; // input
	Point point;
	CMask mask(); // create mask
	mask.load(maskPath); // load mask image (black and white image
	// test getval. Use an image with a known pixel(position, color) to test getval
	// Send back the red value (cause grayscale img have the same value on r, g and b
	point.x = 3;
	point.y = 5;
	cout << mask.getVal();
	// test of border detector + display
	 Border truc = mask.detectMaskBorder();
	 Point point;
	 RGB val;
	 val.r=255;
	 val.g = 0;
	 val.b = 0;
	 for (int i = 0; i < truc.used; i++) {
		 point.x = truc.border[i].Coord.x;
		 point.y = truc.border[i].Coord.y;
		 mask.ChangeVal(point, val); // test changeVal by the way
	 }
	mask.display(); // should display the border in red
	mask.load(maskPath);
	

	return 0;
}*/