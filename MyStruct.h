/*
 *  MaskPoint.h
 *  inpaint
 *
 *  Created by LesKodaux on 11/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>

struct Point{
	int x;
	int y;
};

struct MaskPoint{
	Point Coord;
	float data;
	float confidence;
};

struct Border{
	std::vector<MaskPoint> border;
	int size;
	int used;
};

struct RGB{
	float r;
	float g;
	float b;
};