// File:        main.cpp
// Author:      will
// Date:        2019-11-11
// Description: Partial test of PA3 functionality


#include "QTree.h"

void testClear() {
  PNG im1;
  im1.readFromFile("images/small.png");
  QTree t1(im1,10,RGBAPixel(0,0,0));
}


int main()
{
  // read in an image
  PNG im1;
  im1.readFromFile("images/small.png");
  // use it to build a Qtree with approximately
  // 10 leaves and with each leaf square framed by
  // a black 1-pixel border
  QTree t1(im1,10,RGBAPixel(0,0,0));
  // // write it to a file
  t1.write("images/out-smallFrame.png");
  // // // // use image to build a balanced QTree
  QTree t1b(im1,10,RGBAPixel(0,0,0),true);
  t1b.write("images/out-smallFrameBal.png");

  // // // check copy
  QTree t1copy(t1);
  t1copy.write("images/out-smallFrameCopy.png");

  // // // check assignment
  t1copy = t1b;
  t1copy.write("images/out-smallFrameBalCopy.png");
  
  // // // check clear
  // testClear();
  
  // // read in an image
  PNG im2;
  im2.readFromFile("images/colour.png");
  QTree t2(im2,200,RGBAPixel(0,0,0));
  t2.write("images/out-colourFrame.png");

  // // use image to build a balanced QTree
  // cout << im2.width() << " " << im2.height() << endl;
  QTree t2b(im2,200,RGBAPixel(0,0,0),true);
  t2b.write("images/out-colourFrameBal.png");


  // // read in an image
  PNG im3;
  im3.readFromFile("images/ada.png");
  QTree t3(im3,2000);
  t3.write("images/out-ada.png");

  // // // use image to build a balanced QTree
  QTree t3b(im3,2000,true);
  t3b.write("images/out-adaBal.png");

  // // // read in an image
  PNG im4;
  im4.readFromFile("images/geo.png");
  QTree t4(im4,200);
  t4.write("images/out-geo.png");

  // // // use image to build a balanced QTree
  QTree t4b(im4,200,true);
  t4b.write("images/out-geoBal.png");


  // // // read in an image
  PNG im5;
  im5.readFromFile("images/klee.png");
  QTree t5(im5,5000,RGBAPixel(100,100,100));
  t5.write("images/out-kleeFrame.png");

  // // use image to build a balanced QTree
  QTree t5b(im5,5000,RGBAPixel(100,100,100),true);
  t5b.write("images/out-kleeFrameBal.png");

  return 0;
}
