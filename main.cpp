#include "CImg.h"
#include <iostream>
#include <string>
#include "classMask.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char * argv[]) {
	char *maskPath = "ship-mask.png", *imgPath = "ship-input.png";
	classMask troll;
	CImg<float> mask(maskPath), img(imgPath); // open image. Get a matrix RGB
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