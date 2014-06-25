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
	
	// Testing class CImage
	CImage img;
	img.loadImg(imgPath);
	
 CImage truc;
 truc.loadImg(imgPath);
 truc.blurNGrad();
 truc.displayImg();
 img.displayImg();

 // testing erosion
 erode2 = erode;
 CImgDisplay imgDisp(erode);
 while (!imgDisp.is_closed()){
 imgDisp.wait();
 }
 //Debug
 for (int i = 0; i < erode.width(); i++) {
 for (int j = 0; j < erode.height(); j++) {
 if (erode(i,j,0,0) == 0) {
 erode(i,j,0,0) = 2;
 erode(i,j,0,1) = 2;
 erode(i,j,0,2) = 2;
 
 }
 }
 } 
 //
 CImgDisplay imgDisp2(erode);
 while (!imgDisp2.is_closed()){
 imgDisp2.wait();
 }
 erode2.dilate(3,3,1);
 for (int i = 0; i < erode2.width(); i++) {
 for (int j = 0; j < erode2.height(); j++) {
 if ((erode(i,j,0,0) == 2) and (erode2(i,j,0,0) == 1)) {
 erode2(i,j,0,0) = 2;
 erode2(i,j,0,1) = 2;
 erode2(i,j,0,2) = 2;
 
 }
 cout << erode2(i,j,0,0);
 }
 cout << endl;
 } 
 CImgDisplay imgDisp3(erode2);
 while (!imgDisp3.is_closed()){
 imgDisp3.wait();
 }
	return 0;
}*/