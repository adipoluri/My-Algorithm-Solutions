#include "rgbtree.h"
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "tileUtil.h"
#include <iostream>
#include <map>

#define TILESIZE 16

using namespace std;
using namespace cs221util;
using namespace tiler;


int main()
{

    // read directory and create map of average color -> file name (this function is given)
    map<RGBAPixel, string> photos = buildMap("block/");
    
    // build the kd tree given the photos map.  (you'll implement a rgbtree)
    rgbtree searchStructure(photos);

    // // read a (small, 100x150 or so) target image into timage
    // // tile(timage) returns a TILESIZExwidth by TILESIZExheight image corresponding
    // // to the target.
    PNG timage; timage.readFromFile("customImgs/target.png");

    // // functionality of tile: for each pixel in the target image, find pixel's NN
    // // in the kdtree, returning a photoID. Use the photoID to open the 
    // // correct file, and use that file's pixels in the appropriate place
    // // in the return image.
     PNG mosaic = tile(timage, searchStructure, photos);

     mosaic.writeToFile("customImgs/mosaic.png");

  return 0;
}
