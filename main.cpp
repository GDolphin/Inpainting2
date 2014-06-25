#include "CImg.h"
#include <iostream>
#include <string>
#include "CInpainting.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char * argv[]) {
	char *maskPath = "ship-mask.png", *imgPath = "ship-input.png";
	CImg<float> bidule(maskPath);
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
	img.RGBtoLab(); // convert to lab space
	CImgDisplay imgDisp(img), maskDisp(mask);
	cout << img.width();
	/*while (!imgDisp.is_closed() and !maskDisp.is_closed()){
		imgDisp.wait();
		maskDisp.wait();
	}*/
	return 0;
}