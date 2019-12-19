#include "var.h"
#include <cassert>

double varAndAvg(PNG & im, pair<int,int> ul, int size, RGBAPixel & avg) {
  assert( 0 <= ul.first && ul.first + size <= (int)im.width() );
  assert( 0 <= ul.second && ul.second + size <= (int)im.height() );

  // calculate average over region
  double sumR=0;
  double sumG=0;
  double sumB=0;
  int n=0;
  for (int y = ul.second; y < ul.second + size; y++) {
    for (int x = ul.first; x < ul.first + size; x++) {
      RGBAPixel * p = im.getPixel(x,y);
      sumR += p->r;
      sumG += p->g;
      sumB += p->b;
      ++n;
    }
  }
  avg.r = sumR/(double) n;
  avg.g = sumG/(double) n;
  avg.b = sumB/(double) n;
  avg.a = 255;

  // calculate variance sum
  sumR=0;
  sumG=0;
  sumB=0;
  for (int y = ul.second; y < ul.second + size; y++) {
    for (int x = ul.first; x < ul.first + size; x++) {
      RGBAPixel * p = im.getPixel(x,y);
      sumR += (p->r - avg.r)*(p->r - avg.r);
      sumG += (p->g - avg.g)*(p->g - avg.g);
      sumB += (p->b - avg.b)*(p->b - avg.b);
    }
  }
  //  return (sumR/n + sumG/n + sumB/n);
  return (sumR + sumG + sumB);
}
