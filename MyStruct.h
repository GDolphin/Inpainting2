/*
 *  MaskPoint.h
 *  inpaint
 *
 *  Created by LesKodaux on 11/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef MYSTRUCT_HEADER
#define MYSTRUCT_HEADER
#include <vector>

struct Point{
	int x;
	int y;
};

struct FPoint {
	float x;
	float y;
};

struct MaskPoint{
	Point Coord;
	float data;
	float confidence;
	float priority;
};

struct Border{
	std::vector<MaskPoint> border;
	int size;
	int used;
};

struct UList {
	std::vector<Point> list;
	int size, minX, maxX, minY, maxY;
	Point coord;
};

struct RGB{
	float r;
	float g;
	float b;
};

struct GradStruct {
	RGB x;
	RGB y;
};

#endif // MYSTRUCT_HEADER