/*
 *  CImage.cpp
 *  inpaint
 *
 *  Created by LesKodaux on 12/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "CImage.h"

using namespace cimg_library;
using namespace std;

CImage::CImage(){
};

CImage::~CImage(){
};

void CImage::loadImg(char *imgPath, char *maskPath){
	CImg<float> erode;
	mImg.load(imgPath);
	RGBToLab();
	erode.load(maskPath);
	erode.threshold(128);
	erode.erode(3,3,1);
	computeGrad();
	
	for (int x = 0; x < mImg.width(); x++){
		for (int y = 0; y < mImg.height(); y++) {
			//cout << mImg(x,y,0,0) << " ";
			mGrad[0](x,y,0,0) = mGrad[0](x,y,0,0) * erode(x,y,0,0)/100;
			mGrad[0](x,y,0,1) = mGrad[0](x,y,0,1) * erode(x,y,0,0)/255;
			mGrad[0](x,y,0,2) = mGrad[0](x,y,0,2) * erode(x,y,0,0)/255;
			mGrad[1](x,y,0,0) = mGrad[1](x,y,0,0) * erode(x,y,0,0)/100;
			mGrad[1](x,y,0,1) = mGrad[1](x,y,0,1) * erode(x,y,0,0)/255;
			mGrad[1](x,y,0,2) = mGrad[1](x,y,0,2) * erode(x,y,0,0)/255;
		}
	//cout << endl;
	}
};

void CImage::computeGrad(){
	vector<CImg<float> > var(2);
	CImg<float> img;
	img = mImg;
	var[0] = img;
	var[1] = img;
	
	for (int x =0; x < img.width(); x++) {
		for (int y = 0; y < img.height(); y++) {
			for (int k = 0; k < 3 ; k++) {
				//img(x,y,0,k) = int(img(x,y,0,k));
				var[0](x,y,0,k) = 0;
				var[1](x,y,0,k) = 0;
			}
		}
	}
	img.blur(3,3,1,false,false);
	
	for (int x = 1; x < img.width()-1; x++){
		for (int j = 0; j < 3; j++){
			var[0](x,0,0,j) = (img(x+1,0,0,j) - img(x-1,0,0,j))/2;
			var[0](x,img.height()-1,0,j) = (img(x+1,img.height()-1,0,j) - img(x-1,img.height()-1,0,j))/2;
			var[1](x,0,0,j) = (img(x,1,0,j) - img(x,0,0,j))/2;
			var[1](x,img.height()-1,0,j) = (img(x,img.height()-2,0,j) - img(x-1,img.height()-1,0,j))/2;
		}
		for (int y = 1; y < img.height()-1; y++)
			for (int i = 0; i < 2; i++)
				for (int j =0; j < 3; j++)
					var[i](x,y,0,j) = (img(x+1-i,y+i,0,j) - img(x-1+i,y-i,0,j))/2;
	}
	for (int y = 1; y < img.height()-1; y++)
		for (int j = 0; j < 3; j++){
			var[0](0,y,0,j) = (img(0,y,0,j) - img(1,y,0,j))/2;
			var[0](img.width()-1,0,j) = (img(img.width()-2,y,0,j) - img(img.width()-1,y,0,j))/2;
			var[1](0,y,0,j) = (img(0,y+1,0,j) - img(0,y-1,0,j))/2;
			var[1](img.width()-1,0,j) = (img(img.width()-1,y+1,0,j) - img(img.width()-1,y-1,0,j))/2;
		}
	for (int j = 0; j < 3; j++) {
		var[0](0,0,0,j) = img(1,0,0,j) - img(0,0,0,j);
		var[0](0,img.height()-1,j) =  img(1,img.height()-1,0,j) - img(0,img.height()-1,0,j);
		var[0](img.width()-1,0,0,j) = img(img.width()-2,0,0,j) - img(img.width()-1,0,0,j);
		var[0](img.height()-1,j) =  img(img.width()-2,img.height()-1,0,j) - img(img.width()-1,img.height()-1,0,j);
		
		var[1](0,0,0,j) = img(0,1,0,j) - img(0,0,0,j);
		var[1](0,img.height()-1,j) =  img(0,img.height()-2,0,j) - img(0,img.height()-1,0,j);
		var[1](img.width()-1,0,0,j) = img(img.width()-1,1,0,j) - img(img.width()-1,0,0,j);
		var[1](img.height()-1,j) =  img(img.width()-1,img.height()-2,0,j) - img(img.width()-1,img.height()-1,0,j);
	}
	mGrad = var;
};

void CImage::displayImg(){
	CImgDisplay Disp(mImg);
	while (!Disp.is_closed()){
		Disp.wait();
	}
};

RGB CImage::getVal(Point point){
	RGB val;
	val.r = mImg(point.x,point.y,0,0);
	val.g = mImg(point.x,point.y,0,1);
	val.b = mImg(point.x,point.y,0,2);
	return val;
};

FPoint CImage::getGrad(Point point){
	GradStruct val;
	FPoint retVal;
	val = getGradVal(point);
	retVal.y = sqrt(val.x.r*val.x.r + val.x.g*val.x.g + val.x.b*val.x.b);
	retVal.x = -sqrt(val.y.r*val.y.r + val.y.g*val.y.g + val.y.b*val.y.b);
	return retVal;
};

GradStruct CImage::getGradVal(Point point){
	GradStruct val;
	val.x.r = mGrad[0](point.x,point.y,0,0);
	val.x.g = mGrad[0](point.x,point.y,0,1);
	val.x.b = mGrad[0](point.x,point.y,0,2);
	val.y.r = mGrad[1](point.x,point.y,0,0);
	val.y.g = mGrad[1](point.x,point.y,0,1);
	val.y.b = mGrad[1](point.x,point.y,0,2);
	//mGrad.get_select("truc",true); // display gradient
	return val;
};

void CImage::changeGradVal(Point point, GradStruct val){
	mGrad[0](point.x,point.y,0,0) = val.x.r;
	mGrad[0](point.x,point.y,0,1) = val.x.g;
	mGrad[0](point.x,point.y,0,2) = val.x.b;
	mGrad[1](point.x,point.y,0,0) = val.y.r;
	mGrad[1](point.x,point.y,0,1) = val.y.g;
	mGrad[1](point.x,point.y,0,2) = val.y.b;
};

void CImage::changeVal(Point point,RGB val){
	mImg(point.x, point.y, 0, 0) = val.r;
	mImg(point.x, point.y, 0, 1) = val.g;
	mImg(point.x, point.y, 0, 2) = val.b;
};

void CImage::LabToRGB(){
	//LabToXYZ();
	//XYZToRGB();
	mImg.LabtoRGB(); // method != function below
	mImg.RGBtosRGB();
};

void CImage::RGBToLab(){
	mImg.sRGBtoRGB();
	mImg.RGBtoLab(); // method != function below

	
	
	
};

void CImage::RGBToXYZ(){
	float X = 0, Y=0, Z=0;
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			X = 2.768892*mImg(x,y,0,0) + 1.751748*mImg(x,y,0,1) + 1.1302*mImg(x,y,0,2);
			Y = mImg(x,y,0,0) + 4.5907*mImg(x,y,0,1) + 0.0601*mImg(x,y,0,2);
			Z = 0.056508*mImg(x,y,0,1) + 5.594292*mImg(x,y,0,2);
			mImg(x,y,0,0) = X;
			mImg(x,y,0,1) = Y;
			mImg(x,y,0,2) = Z;
		}
	}
};

void CImage::XYZToRGB(){
	float R = 0 , G = 0, B = 0;
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			R = 0.418456 * mImg(x,y,0,0) - 0.158657 * mImg(x,y,0,1) - 0.082833 * mImg(x,y,0,2);
			G = -0.091167 * mImg(x,y,0,0) + 0.252426 * mImg(x,y,0,1) + 0.015707 * mImg(x,y,0,2);
			B = 0.000921 * mImg(x,y,0,0) - 0.00255 * mImg(x,y,0,1) + 0.178595 * mImg(x,y,0,2);
			mImg(x,y,0,0) = R;
			mImg(x,y,0,1) = G;
			mImg(x,y,0,2) = B;
		}
	}
};

void CImage::XYZToLab(){
	float L = 0, a = 0, b = 0;
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			L = 116*f(mImg(x,y,0,1)/(100));
			a = 500*(f(mImg(x,y,0,0)/(95.05))-f(mImg(x,y,0,1)/(100)));
			b = 200*(f(mImg(x,y,0,1)/(100))-f(mImg(x,y,0,2)/(108.9)));
			mImg(x,y,0,0) = L;
			mImg(x,y,0,1) = a;
			mImg(x,y,0,2) = b;
		}
	}
};

void CImage::LabToXYZ(){
	float X = 0, Y = 0 , Z = 0;
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			Y = 100*fInv((mImg(x,y,0,0)+16)/116);
			X = 95*fInv((mImg(x,y,0,0)+16)/116+mImg(x,y,0,1)/500);
			Z = 108.9*fInv((mImg(x,y,0,0)+16)/116-mImg(x,y,0,2)/200);
			mImg(x,y,0,0) = X;
			mImg(x,y,0,1) = Y;
			mImg(x,y,0,2) = Z;
		}
	}
};

float CImage::f(float n){
	float val = (6/29)^3;
	if(n > val)
		return pow(n, 1/3);
	else
		return 1/3*29/6*29/6*n+4/29;

};

float CImage::fInv(float n){
	float val = (6/29);
	if (n > val)
		return pow(n, 3);
	else
		return 3*val*val*(n-4/29);
};