#ifndef _VAR_H
#define _VAR_H

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include <utility>
#include <cmath>
using namespace std;
using namespace cs221util;

#define PI 3.14159265

/** Given a square, return the color variance over the square and
 *  the average pixel (returned via a reference parameter).
 *  @param ul is (x,y) of the upper left corner of the rectangle 
 *  @param size is the side length of the square
 **/
double varAndAvg(PNG & im, pair<int,int> ul, int size, RGBAPixel & avg);


#endif
