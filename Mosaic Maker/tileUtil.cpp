#include "tileUtil.h"

/**
 * Function tile:
 * @param PNG & target: an image to use as base for the mosaic. it's pixels will be
 *                      be replaced by thumbnail images whose average color is close
 *                      to the pixel.
 * @param rgbtree & ss: a kd-tree of RGBAPixels, used as a query structure for
 *                      nearest neighbor search. 
 * @param map<RGBAPixel, string> & photos: a map that takes a color key and returns the
 *                      filename of an image whose average color is that key.
 *
 * returns: a PNG whose dimensions are TILESIZE times that of the target. Each
 * pixel in the target is used as a query to ss.findNearestNeighbor, and the response
 * is used as a key in photos. 
 */

PNG tiler::tile(PNG & target, const rgbtree & ss, map<RGBAPixel,string> & photos){
    PNG result(TILESIZE*target.width(), TILESIZE*target.height());
    for(unsigned int i = 0; i < target.width(); i++) {
        for(unsigned int j = 0; j < target.height(); j++) {
            RGBAPixel temp = ss.findNearestNeighbor(*target.getPixel(i,j));
            PNG tile;
            tile.readFromFile(photos[temp]);
            for(unsigned int x = 0; x < tile.width(); x++) {
                for(unsigned int y = 0; y < tile.height(); y++) {
                    RGBAPixel * pix = result.getPixel(i*TILESIZE + x,j*TILESIZE + y);
                    RGBAPixel curr = (*tile.getPixel(x,y));
                    (*pix) = curr;
                }
            }
        }
    }
    return result;

}

/* buildMap: function for building the map of <key, value> pairs, where the key is an
 * RGBAPixel representing the average color over an image, and the value is 
 * a string representing the path/filename.png of the TILESIZExTILESIZE image
 * whose average color is the key.
 * 
 * @param path is the subdirectory in which the tiles can be found. In our examples
 * this is imlib.
 *
*/
map<RGBAPixel, string> tiler::buildMap(string path) {

    map < RGBAPixel, string> thumbs;
    for (const auto & entry : fs::directory_iterator(path)) {
        int r = 0;
        int g = 0;
        int b = 0;
        PNG curr; curr.readFromFile(entry.path());
        int resizeratio = curr.width()*curr.height();
        for(unsigned int i = 0; i < curr.width(); i++) {
            for(unsigned int j = 0; j < curr.height(); j++) {
                RGBAPixel* temp = curr.getPixel(i,j);
                r += (int) temp->r;
                g += (int) temp->g;
                b += (int) temp->b;
            }
        }
        r = r/resizeratio;
        g = g/resizeratio;
        b = b/resizeratio;

        thumbs[RGBAPixel(r,g,b)] = entry.path();

    }
    return thumbs;
}


