/**
 * @file rgbtree.cpp
 * Implementation of rgbtree class.
 */

#include <utility>
#include <algorithm>
#include "rgbtree.h"

using namespace std;

rgbtree::rgbtree(const map<RGBAPixel,string>& photos) {
  for (auto & key : photos) {
    tree.push_back(key.first);
  }
  treeSize = photos.size();
  treeHelper(0, treeSize - 1, 0);
}


void rgbtree::treeHelper(int start, int end, int d) {
  if(start >= end) {
    return;
  }
  int k = (start + end)/2;
  quickSelect(start, end, k, d);
  treeHelper(start, k-1, d+1);
  treeHelper(k+1, end, d+1);
}



RGBAPixel rgbtree::findNearestNeighbor(const RGBAPixel & query) const {
    return tree[NearestNeighborHelper(query, 0, treeSize - 1, 0, (treeSize - 1)/2)];
}


int rgbtree::NearestNeighborHelper(const RGBAPixel & query, int start, int end, int curDim, int bestSoFar) const {
  if(start > end){
      return bestSoFar;
  }

  int median = (start + end)/2;
  RGBAPixel curr = tree[median];
  int best;
  int tempBest;

  if(absDist(tree[bestSoFar], query) > absDist(tree[median], query)) {
    tempBest = median;
  } else {
    tempBest = bestSoFar;
  }


  if(smallerByDim(query, curr, curDim)) {
    best = NearestNeighborHelper(query, start, median - 1, curDim + 1, tempBest);

    if(distToSplit(query, curr, curDim) < absDist(tree[best], query)) {
      int newBest = NearestNeighborHelper(query, median + 1, end, curDim + 1, best);
      if(absDist(tree[best],query) > absDist(tree[newBest], query)) {
        best = newBest;
      }
    }
  } else {
    best = NearestNeighborHelper(query, median + 1, end, curDim + 1, tempBest);
    
    if(distToSplit(query, curr, curDim) < absDist(tree[best], query)) {
      int newBest = NearestNeighborHelper(query, start, median - 1, curDim + 1, best);
      if(absDist(tree[best],query) > absDist(tree[newBest], query)) {
        best = newBest;
      }
    }
  }

  return best;
}



/*Returns True if RGB Pixel first is smaller than Pixel second with respect to dimension "CurDim"
 *Ex. (3,4,1) is smallerByDim than (2,3,4) if the dimension is 2 (0 indexed)
 */
bool rgbtree::smallerByDim(const RGBAPixel & first,
                                const RGBAPixel & second, int curDim) const
{

  if(curDim%3 == 0) {
    if((int) first.r == (int) second.r) {return first.operator<(second);}
    return (int) first.r < (int) second.r;
  } else if (curDim%3 == 1) {
    if((int) first.g == (int) second.g) {return first.operator<(second);}
    return (int) first.g < (int) second.g;
  } else {
    if((int) first.b == (int) second.b) {return first.operator<(second);}
    return (int) first.b < (int) second.b;
  }

}

/*
 * This function splits the trees[start..end] subarray at position start k
 */
void rgbtree::quickSelect(int start, int end, int k, int d)
{
  if(start > end) {
    return;
  }
  int p = partition(start, end, d);
  if(k < p) { 
     quickSelect(start, p-1, (start + (p-1))/2, d);
  }
  if (k > p) {
    quickSelect(p+1, end,  (end + (p+1))/2, d);
  }
}


/**
 * This method does a partition around pivot and will be used 
 * in quick select. It uses tree[lo] as the default pivot.
 * It returns the index of the pivot in the updated vector.
 * You will likely need to modify and complete this code.
 */
int rgbtree::partition(int lo, int hi, int d) 
{
    int p = lo;
    for( int i=lo+1; i <= hi; i++ ) {
      if(smallerByDim(tree[i], tree[lo], d)) {
          p++; 
          swap(tree[p], tree[i]);
      }
    }
    swap(tree[lo], tree[p]);
    return p;

}

//Return absolute 3-dimensional distance between two pixels (represented as points)
int rgbtree::absDist(const RGBAPixel& query, const RGBAPixel& curr) const {
  return ((int)query.r - (int)curr.r)*((int)query.r - (int)curr.r) + ((int)query.g - (int)curr.g)*((int)query.g - (int)curr.g) + ((int)query.b - (int)curr.b)*((int)query.b - (int)curr.b);
}

/**
 * Helper function to help determine if the nearest neighbor could 
 * be on the other side of the KD tree.
 */
int rgbtree::distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int dimension) const
{

  if(dimension%3 == 0) {
    return ((int) query.r - (int) curr.r)*((int) query.r - (int) curr.r);
  } else if (dimension%3 == 1) {
    return ((int) query.g - (int) curr.g)*((int) query.g - (int) curr.g);
  } else {
    return ((int) query.b - (int) curr.b)*((int) query.b - (int) curr.b);
  }

}

