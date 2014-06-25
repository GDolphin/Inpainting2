#include "CImg.h"
#include <iostream>
#include <string>
#include "CInpainting.h"
//#include "CImage.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char * argv[]) {
	char *maskPath = "bungee-mask.jpg", *imgPath = "bungee-input.jpg";
	/*CImg<float>troll(maskPath);
	
	CImgDisplay imgDisp(troll);
	troll.erode(10,20,1);
	CImgDisplay imgDisp2(troll);
	while (!imgDisp.is_closed()){
	 imgDisp.wait();
		imgDisp2.wait();
	 }*/
	CInpainting truc(maskPath, imgPath, 3,3);
	truc.inpaint();
		/*CImg<float> bidule(maskPath);
	CInpainting troll;
	troll.loadMask(maskPath);
	troll.detectMaskBorder();
	troll.computeConfidence();
	//vector<int> a;
	//a.reserve(10);
	//a.resize(10);
	//troll.displayMask();
	/*CImg<float> mask(maskPath), img(imgPath); // open image. Get a matrix RGB
	mask.threshold(128);
	img.RGBtoLab(); // convert to lab space*/
	
	return 0;
}