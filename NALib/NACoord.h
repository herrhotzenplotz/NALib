
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_COORD_INCLUDED
#define NA_COORD_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

// Pos, Size and Rect
// Following are definitions for integer and floating-point elements storing
// a position, a size and a rectangle.
//
// When coordinate functions are used, they usually are called many times.
// In NALib, coordinate computation is always performed straight-forward. Even
// if the values don't make sense.
//
// When debugging, a lot of checks are performed by NALib to find invalid
// values but checking for invalid values when not debugging would increase
// runtime considerably. The implementation would also become very complicated
// as there are many special cases.
//
// The programmer has to take care of invalid values. Use the debug checks
// and a few helper functions. This should be doable and in the end helps
// avoiding nasty errors which are not uncommon with coordinate computations.
//
// Following are the definitions of valid elements used in NALib:
// Pos:  A position is considered valid if none of the fields is NaN.
// Size: A size is considered valid, if both fields are positive or zero.
//       A size of zero is considered valid because this is commonly used to
//       mark empty structures. But be careful! It is just NOT DEFINED as
//       invalid. This means: No special code is executed for empty elements!
//       Use the appropriate functions to check if an element is empty.
// Rect: A rectangle is considered valid, if its position and size is valid.
//
// Sizes or rects with zeros are considered "empty". But no special code will
// be executed! This might lead to unexpected behaviour. When debugging, NALib
// checks if a size or rect is empty where it should not be. But you as the
// programmer have to take care of that in your code! 
//
// Negative sizes are not uncommon as well. A rect with a negative size can for
// example be generated with naMakeRectWithStartAndEnd where the start and end
// points are reversed. While not forbidden and perfectly meaningful, most
// function only work properly with positive rects. Use the naMakeRectPositive
// function to convert a negative (and hence invalid) rect into a positive one.
//
// Also note that the integer variant of size and rect behaves differently than
// the floating-point variant. The floating-point-variant treats sizes like
// "inclusive". For example Rect(Pos(0., 0.), Size(5., 5.)) contains the point
// Pos(5., 5.). The integer Rect(Pos(0, 0), Size(5, 5)) on the other hand only
// contains Pos(4, 4) but not Pos(5, 5).
//
// The integer variants are good for indexes. Therefore, the integer variants
// of NARect denote both a "Max" and an "End". The "End" is computed pos+width.
// The "Max" is computed End-1. For example, use the End for the terminating
// condition of a for-loop. Use the Max to access the last element.

#include "NAMathConstants.h"
#include "NAMathOperators.h"
#include "NARange.h"

typedef struct NAPos NAPos;
struct NAPos{
  double x;
  double y;
};
typedef struct NASize NASize;
struct NASize{
  double width;
  double height;
};
typedef struct NARect NARect;
struct NARect{
  NAPos  pos;
  NASize size;
};
typedef struct NAPosi NAPosi;
struct NAPosi{
  NAInt x;
  NAInt y;
};
typedef struct NASizei NASizei;
struct NASizei{
  NAInt width;
  NAInt height;
};
typedef struct NARecti NARecti;
struct NARecti{
  NAPosi  pos;
  NASizei size;
};


// Create the various elements
NA_IAPI NAPos     naMakePos     (double x,      double  y);
NA_IAPI NASize    naMakeSize    (double width,  double  height);
NA_IAPI NARect    naMakeRect    (NAPos  pos,    NASize  size);
NA_IAPI NAPosi    naMakePosi    (NAInt  x,      NAInt   y);
NA_IAPI NASizei   naMakeSizei   (NAInt  width,  NAInt   height);
NA_IAPI NARecti   naMakeRecti   (NAPosi pos,    NASizei size);

// Create rects with start- end end-points. Note that the integer variant
// either expects the max or the end value (max = end-1).
NA_IAPI NARect    naMakeRectWithStartAndEnd (NAPos  start, NAPos  end);
NA_IAPI NARecti   naMakeRectiWithStartAndEnd(NAPosi start, NAPosi end);
NA_IAPI NARecti   naMakeRectiWithStartAndMax(NAPosi start, NAPosi max);

// Create the bounding box of two elements.
NA_IAPI NARect    naMakeRectWithRectAndPos    (NARect  rect,  NAPos   pos);
NA_IAPI NARect    naMakeRectWithRectAndRect   (NARect  rect1, NARect  rect2);
NA_IAPI NARecti   naMakeRectiWithRectiAndPosi (NARecti rect,  NAPosi  pos);
NA_IAPI NARecti   naMakeRectiWithRectiAndRecti(NARecti rect1, NARecti rect2);

// Clamping functions. Returns the element which is completely within another.
NA_IAPI NAPos     naClampPosToRect(NAPos pos, NARect clamprect);
NA_IAPI NARect    naClampRectToRect(NARect rect, NARect clamprect);
NA_IAPI NAPosi    naClampPosiToRecti(NAPosi pos, NARecti clamprect);
NA_IAPI NARecti   naClampRectiToRecti(NARecti rect, NARecti clamprect);

// The same but wiht real and integer mixed.
NA_IAPI NAPos     naClampPosToRecti(NAPos pos, NARecti clamprect);

// Use the following functions to make equality tests (==) between elements.
NA_IAPI NABool    naEqualPos  (NAPos   pos1,  NAPos   pos2);
NA_IAPI NABool    naEqualSize (NASize  size1, NASize  size2);
NA_IAPI NABool    naEqualRect (NARect  rect1, NARect  rect2);
NA_IAPI NABool    naEqualPosi (NAPosi  pos1,  NAPosi  pos2);
NA_IAPI NABool    naEqualSizei(NASizei size1, NASizei size2);
NA_IAPI NABool    naEqualRecti(NARecti rect1, NARecti rect2);

// Containing functions. Test if an element is within another.
NA_IAPI NABool    naIsPosInSize   (NAPos   pos,  NASize  outersize);
NA_IAPI NABool    naIsPosInRect   (NAPos   pos,  NARect  outerrect);
NA_IAPI NABool    naIsSizeInSize  (NASize  size, NASize  outersize);
NA_IAPI NABool    naIsRectInRect  (NARect  rect, NARect  outerrect);
NA_IAPI NABool    naIsPosiInSizei (NAPosi  pos,  NASizei outersize);
NA_IAPI NABool    naIsPosiInRecti (NAPosi  pos,  NARecti outerrect);
NA_IAPI NABool    naIsSizeiInSizei(NASizei size, NASizei outersize);
NA_IAPI NABool    naIsRectiInRecti(NARecti rect, NARecti outerrect);

// Get the End-Point of a rect. Is the same as pos + size.
NA_IAPI double    naGetRectEndX (NARect  rect);
NA_IAPI double    naGetRectEndY (NARect  rect);

// Index functions of NARecti. Treats the rect as a description of a
// 2-dimensional array with row-first ordering.
// End returns position + size
// Max returns position + size - 1
// Count returns the total count width*height
// IndexOf returns the index of the corresponding 1-dimensional array.
NA_IAPI NAInt     naGetRectiEndX        (NARecti rect);
NA_IAPI NAInt     naGetRectiEndY        (NARecti rect);
NA_IAPI NAInt     naGetRectiMaxX        (NARecti rect);
NA_IAPI NAInt     naGetRectiMaxY        (NARecti rect);
NA_IAPI NAInt     naGetRectiIndexCount  (NARecti rect);
NA_IAPI NAInt     naGetRectiIndexOfPosi (NARecti rect, NAPosi pos);
// Example:    Rect(Pos(2, 5), Size(4, 3)) describes a 2D-field. It stores
//             a corresponding 1D-Array {a, b, c, d, e, f, g, h, i, j, k, l}.
// (a b c d)   End is (6, 8), Max is (5, 7). Count is 3*4 = 12
// (e f g h)   The result of naGetRectiIndexOfPosi with position (5, 6)
// (i j k l)   returns 7, the index of 'h'

// Returns NA_TRUE when all values of the fields are valid. Will not perform
// any debugging tests.
NA_IAPI NABool    naIsPosValid  (NAPos   pos);
NA_IAPI NABool    naIsSizeValid (NASize  size);
NA_IAPI NABool    naIsRectValid (NARect  rect);
NA_IAPI NABool    naIsPosiValid (NAPosi  pos);
NA_IAPI NABool    naIsSizeiValid(NASizei size);
NA_IAPI NABool    naIsRectiValid(NARecti rect);

// Sizes with zero-entries.  Will not perform any debugging tests.
NA_IAPI NABool    naIsSizeEmpty (NASize  size);
NA_IAPI NABool    naIsRectEmpty (NARect  rect);
NA_IAPI NABool    naIsSizeiEmpty(NASizei size);
NA_IAPI NABool    naIsRectiEmpty(NARecti rect);

// positive and negative sizes and rects. Will not perform any debugging tests.
NA_IAPI NABool    naIsSizePositive (NASize  size);
NA_IAPI NABool    naIsRectPositive (NARect  rect);
NA_IAPI NABool    naIsSizeiPositive(NASizei size);
NA_IAPI NABool    naIsRectiPositive(NARecti rect);

// The naMakeRectPositive function ensures that the sizes of the given rect
// are positive. If the sizes already were positive, they will not be changed.
// For example:  A rect with pos(4., 6.) and size (-2., 7.) will change to
//            -> a rect with pos(2., 6.) and size ( 2., 7.) .
// But: An integer rect with pos(4 , 6 ) and size (-2 , 7 ) will change to
//            -> a rect with pos(3 , 6 ) and size ( 2 , 7 ) !
// Will not perform any debugging tests.
NA_IAPI NARect    naMakeRectPositive (NARect  rect);
NA_IAPI NARecti   naMakeRectiPositive(NARecti rect);















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////



// helper functions. Should be hidden. todo.
NA_IAPI NABool naIsPosFieldValid(double a){
  return !naIsNaN(a);
}
NA_IAPI NABool naIsPosiFieldValid(NAInt a){
  return NA_TRUE;
}
NA_IAPI NABool naIsSizeFieldValid(double a){
  return (a >= 0.);
}
NA_IAPI NABool naIsSizeiFieldValid(NAInt a){
  return (a >= 0);
}



NA_IAPI NAPos naMakePos(double x, double y){
  #ifndef NDEBUG
    if(!(naIsPosFieldValid(x) && naIsPosFieldValid(y)))
      naError("naMakePos", "Invalid values given.");
  #endif
  NAPos newpos = {x, y};
  return newpos;
}
NA_IAPI NASize naMakeSize(double width, double height){
  #ifndef NDEBUG
    if(!(naIsSizeFieldValid(width) && naIsSizeFieldValid(height)))
      naError("naMakeSize", "Invalid values given.");
  #endif
  NASize newsize = {width, height};
  return newsize;
}
NA_IAPI NARect naMakeRect(NAPos pos, NASize size){
  #ifndef NDEBUG
    if(!naIsPosValid(pos) || !naIsSizeValid(size))
      naError("naMakeRect", "Invalid values given.");
  #endif
  NARect newrect = {pos, size};
  return newrect;
}
NA_IAPI NAPosi naMakePosi(NAInt x, NAInt y){
  #ifndef NDEBUG
    if(!(naIsPosiFieldValid(x) && naIsPosiFieldValid(y)))
      naError("naMakePosi", "Invalid values given.");
  #endif
  NAPosi newpos = {x, y};
  return newpos;
}
NA_IAPI NASizei naMakeSizei(NAInt width, NAInt height){
  #ifndef NDEBUG
    if(!(naIsSizeiFieldValid(width) && naIsSizeiFieldValid(height)))
      naError("naMakeSizei", "Invalid values given.");
  #endif
  NASizei newsize = {width, height};
  return newsize;
}
NA_IAPI NARecti naMakeRecti(NAPosi pos, NASizei size){
  #ifndef NDEBUG
    if(!naIsPosiValid(pos) || !naIsSizeiValid(size))
      naError("naMakeRecti", "Invalid values given.");
  #endif
  NARecti newrect = {pos, size};
  return newrect;
}


NA_IAPI NARect naMakeRectWithStartAndEnd(NAPos start, NAPos end){
  #ifndef NDEBUG
    if(!naIsPosValid(start))
      naError("naMakeRectWithStartAndEnd", "start is invalid.");
    if(!naIsPosValid(end))
      naError("naMakeRectWithStartAndEnd", "end is invalid.");
  #endif
  NARect newrect;
  newrect.pos = start;
  newrect.size.width  = end.x - start.x;
  newrect.size.height = end.y - start.y;
  return newrect;
}
NA_IAPI NARecti naMakeRectiWithStartAndEnd(NAPosi start, NAPosi end){
  #ifndef NDEBUG
    if(!naIsPosiValid(start))
      naError("naMakeRectiWithStartAndEnd", "start is invalid.");
    if(!naIsPosiValid(end))
      naError("naMakeRectiWithStartAndEnd", "end is invalid.");
  #endif
  NARecti newrect;
  newrect.pos = start;
  newrect.size.width  = end.x - start.x;
  newrect.size.height = end.y - start.y;
  return newrect;
}
NA_IAPI NARecti naMakeRectiWithStartAndMax(NAPosi start, NAPosi max){
  #ifndef NDEBUG
    if(!naIsPosiValid(start))
      naError("naMakeRectiWithStartAndMax", "start is invalid.");
    if(!naIsPosiValid(max))
      naError("naMakeRectiWithStartAndMax", "max is invalid.");
  #endif
  NARecti newrect;
  newrect.pos = start;
  newrect.size.width  = (max.x - start.x) + 1;
  newrect.size.height = (max.y - start.y) + 1;
  return newrect;
}


NA_IAPI NARect naMakeRectWithRectAndPos(NARect rect, NAPos pos){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naMakeRectWithRectAndPos", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naMakeRectWithRectAndPos", "rect is invalid.");
    if(!naIsPosValid(pos))
      naError("naMakeRectWithRectAndPos", "pos is invalid.");
  #endif
  NARect newrect;
  NAInt end;
  newrect.pos.x = naMin(rect.pos.x, pos.x);
  newrect.pos.y = naMin(rect.pos.y, pos.y);
  end = naGetRectEndX(rect);
  newrect.size.width  = (naMax(end, pos.x) - newrect.pos.x);
  end = naGetRectEndY(rect);
  newrect.size.height  = (naMax(end, pos.y) - newrect.pos.y);
  return newrect;
}


NA_IAPI NARect naMakeRectWithRectAndRect(NARect rect1, NARect rect2){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect1))
      naError("naMakeRectWithRectAndRect", "rect1 is empty.");
    if(!naIsRectValid(rect1))
      naError("naMakeRectWithRectAndRect", "rect1 is invalid.");
    if(naIsRectEmpty(rect2))
      naError("naMakeRectWithRectAndRect", "rect2 is empty.");
    if(!naIsRectValid(rect2))
      naError("naMakeRectWithRectAndRect", "rect2 is invalid.");
  #endif
  NARect newrect;
  NAInt end1;
  NAInt end2;
  newrect.pos.x = naMin(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMin(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectEndX(rect1);
  end2 = naGetRectEndX(rect2);
  newrect.size.width  = naMax(end1, end2) - newrect.pos.x;
  end1 = naGetRectEndY(rect1);
  end2 = naGetRectEndY(rect2);
  newrect.size.height  = naMax(end1, end2) - newrect.pos.y;
  return newrect;
}


NA_IAPI NARecti naMakeRectiWithRectiAndPosi(NARecti rect, NAPosi pos){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naMakeRectiWithRectiAndPosi", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naMakeRectiWithRectiAndPosi", "rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("naMakeRectiWithRectiAndPosi", "pos is invalid.");
  #endif
  NARecti newrect;
  NAInt end;
  newrect.pos.x = naMin(rect.pos.x, pos.x);
  newrect.pos.y = naMin(rect.pos.y, pos.y);
  end = naGetRectiEndX(rect);
  newrect.size.width  = (naMaxi(end, pos.x + 1) - newrect.pos.x);
  end = naGetRectiEndY(rect);
  newrect.size.height  = (naMaxi(end, pos.y + 1) - newrect.pos.y);
  return newrect;
  // Have you noted the addition plus 1? It's important!
}


NA_IAPI NARecti naMakeRectiWithRectiAndRecti(NARecti rect1, NARecti rect2){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect1))
      naError("naMakeRectiWithRectiAndRecti", "rect1 is empty.");
    if(!naIsRectiValid(rect1))
      naError("naMakeRectiWithRectiAndRecti", "rect1 is invalid.");
    if(naIsRectiEmpty(rect2))
      naError("naMakeRectiWithRectiAndRecti", "rect2 is empty.");
    if(!naIsRectiValid(rect2))
      naError("naMakeRectiWithRectiAndRecti", "rect2 is invalid.");
  #endif
  NARecti newrect;
  NAInt end1;
  NAInt end2;
  newrect.pos.x = naMini(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMini(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectiEndX(rect1);
  end2 = naGetRectiEndX(rect2);
  newrect.size.width  = naMaxi(end1, end2) - newrect.pos.x;
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newrect.size.height  = naMaxi(end1, end2) - newrect.pos.y;
  return newrect;
}



NA_IAPI NAPos naClampPosToRect(NAPos pos, NARect clamprect){
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naClampPosToRect", "pos is invalid.");
    if(naIsRectEmpty(clamprect))
      naError("naClampPosToRect", "clamprect is empty.");
    if(!naIsRectValid(clamprect))
      naError("naClampPosToRect", "clamprect is invalid.");
  #endif
  double end;
  NAPos newpos = pos;
  if(newpos.x < clamprect.pos.x) {newpos.x = clamprect.pos.x;}
  end = naGetRectEndX(clamprect);
  if(newpos.x > end){newpos.x = end;}
  if(newpos.y < clamprect.pos.y) {newpos.y = clamprect.pos.y;}
  end = naGetRectEndY(clamprect);
  if(newpos.y > end){newpos.y = end;}
  return newpos;
}


NA_IAPI NARect naClampRectToRect(NARect rect, NARect clamprect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naClampRectToRect", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naClampRectToRect", "rect is invalid.");
    if(naIsRectEmpty(clamprect))
      naError("naClampRectToRect", "clamprect is empty.");
    if(!naIsRectValid(clamprect))
      naError("naClampRectToRect", "clamprect is invalid.");
  #endif

  double value;
  NARect newrect = rect;
  
  // Adjust horizontally
  value = clamprect.pos.x - newrect.pos.x;
  if(value > 0){
    newrect.size.width -= value;
    newrect.pos.x = clamprect.pos.x;
  }
  value = naGetRectEndX(clamprect);
  if(naGetRectEndX(newrect) > value){
    newrect.size.width = value - newrect.pos.x;
  }
  
  // Adjust vertically
  value = clamprect.pos.y - newrect.pos.y;
  if(value > 0){
    newrect.size.height -= value;
    newrect.pos.y = clamprect.pos.y;
  }
  value = naGetRectEndY(clamprect);
  if(naGetRectEndY(newrect) > value){
    newrect.size.height = value - newrect.pos.y;
  }
  return newrect;
}


NA_IAPI NAPosi naClampPosiToRecti(NAPosi pos, NARecti clamprect){
  #ifndef NDEBUG
    if(!naIsPosiValid(pos))
      naError("naClampPosiToRecti", "pos is invalid.");
    if(naIsRectiEmpty(clamprect))
      naError("naClampPosiToRecti", "clamprect is empty.");
    if(!naIsRectiValid(clamprect))
      naError("naClampPosiToRecti", "clamprect is invalid.");
  #endif
  NAInt max;
  NAPosi newpos = pos;
  if(newpos.x <  clamprect.pos.x) {newpos.x = clamprect.pos.x;}
  max = naGetRectiMaxX(clamprect);
  if(newpos.x > max){newpos.x = max;}
  if(newpos.y <  clamprect.pos.y) {newpos.y = clamprect.pos.y;}
  max = naGetRectiMaxY(clamprect);
  if(newpos.y > max){newpos.y = max;}
  return newpos;
}


NA_IAPI NARecti naClampRectiToRecti(NARecti rect, NARecti clamprect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naClampRectiToRecti", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naClampRectiToRecti", "rect is invalid.");
    if(naIsRectiEmpty(clamprect))
      naError("naClampRectiToRecti", "clamprect is empty.");
    if(!naIsRectiValid(clamprect))
      naError("naClampRectiToRecti", "clamprect is invalid.");
  #endif

  NAInt value;
  NARecti newrect = rect;
    
  // Adjust horizontally
  value = clamprect.pos.x - newrect.pos.x;
  if(value > 0){
    newrect.size.width -= value;
    newrect.pos.x = clamprect.pos.x;
  }
  value = naGetRectiEndX(clamprect);
  if(naGetRectiEndX(newrect) > value){
    newrect.size.width = value - newrect.pos.x;
  }
  
  // Adjust vertically
  value = clamprect.pos.y - newrect.pos.y;
  if(value > 0){
    newrect.size.height -= value;
    newrect.pos.y = clamprect.pos.y;
  }
  value = naGetRectiEndY(clamprect);
  if(naGetRectiEndY(newrect) > value){
    newrect.size.height = value - newrect.pos.y;
  }
  return newrect;
}



NA_IAPI NAPos naClampPosToRecti(NAPos pos, NARecti clamprect){
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naClampPosToRecti", "pos is invalid.");
    if(naIsRectiEmpty(clamprect))
      naError("naClampPosToRecti", "clamprect is empty.");
    if(!naIsRectiValid(clamprect))
      naError("naClampPosToRecti", "clamprect is invalid.");
  #endif
  NAInt max;
  NAPos newpos = pos;
  if(newpos.x <  clamprect.pos.x) {newpos.x = clamprect.pos.x;}
  max = naGetRectiMaxX(clamprect);
  if(newpos.x > max){newpos.x = max;}
  if(newpos.y <  clamprect.pos.y) {newpos.y = clamprect.pos.y;}
  max = naGetRectiMaxY(clamprect);
  if(newpos.y > max){newpos.y = max;}
  return newpos;
}



NA_IAPI NABool naEqualPos(NAPos pos1, NAPos pos2){
  #ifndef NDEBUG
    if(!naIsPosValid(pos1))
      naError("naEqualPos", "pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("naEqualPos", "pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}
NA_IAPI NABool naEqualSize(NASize size1, NASize size2){
  #ifndef NDEBUG
    if(naIsSizeEmpty(size1) || naIsSizeEmpty(size2))
      naError("naEqualSize", "Equality test not valid for empty sizes.");
    if(!naIsSizeValid(size1))
      naError("naEqualSize", "size1 is invalid.");
    if(!naIsSizeValid(size2))
      naError("naEqualSize", "size2 is invalid.");
  #endif
  return ((size1.width == size2.width) && (size1.height == size2.height));
}
NA_IAPI NABool naEqualRect(NARect rect1, NARect rect2){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect1) || naIsRectEmpty(rect2))
      naError("naEqualRect", "Equality test not valid for empty rects.");
    if(!naIsRectValid(rect1))
      naError("naEqualRect", "rect1 is invalid.");
    if(!naIsRectValid(rect2))
      naError("naEqualRect", "rect2 is invalid.");
  #endif
  return     (naEqualPos (rect1.pos,  rect2.pos)
          &&  naEqualSize(rect1.size, rect2.size));
}
NA_IAPI NABool naEqualPosi(NAPosi pos1, NAPosi pos2){
  #ifndef NDEBUG
    if(!naIsPosiValid(pos1))
      naError("naEqualPosi", "pos1 is invalid.");
    if(!naIsPosiValid(pos2))
      naError("naEqualPosi", "pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}
NA_IAPI NABool naEqualSizei(NASizei size1, NASizei size2){
  #ifndef NDEBUG
    if(naIsSizeiEmpty(size1) || naIsSizeiEmpty(size2))
      naError("naEqualSizei", "Equality test not valid for empty sizes.");
    if(!naIsSizeiValid(size1))
      naError("naEqualSizei", "size1 is invalid.");
    if(!naIsSizeiValid(size2))
      naError("naEqualSizei", "size2 is invalid.");
  #endif
  return ((size1.width == size2.width) && (size1.height == size2.height));
}
NA_IAPI NABool naEqualRecti(NARecti rect1, NARecti rect2){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect1) || naIsRectiEmpty(rect2))
      naError("naEqualRecti", "Equality test not valid for empty rects.");
    if(!naIsRectiValid(rect1))
      naError("naEqualRecti", "rect1 is invalid.");
    if(!naIsRectiValid(rect2))
      naError("naEqualRecti", "rect2 is invalid.");
  #endif
  return    (naEqualPosi (rect1.pos,  rect2.pos)
          && naEqualSizei(rect1.size, rect2.size));
}


NA_IAPI NABool naIsPosInSize(NAPos pos, NASize outersize){
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naIsPosInSize", "pos is invalid.");
    if(naIsSizeEmpty(outersize))
      naError("naIsPosInSize", "Inside test not valid for empty sizes.");
    if(!naIsSizeValid(outersize))
      naError("naIsPosInSize", "outersize is invalid.");
  #endif
  return !((pos.x < 0)
        || (pos.x > outersize.width)
        || (pos.y < 0)
        || (pos.y > outersize.height));
}
NA_IAPI NABool naIsPosInRect(NAPos pos, NARect outerrect){
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naIsPosInRect", "pos is invalid.");
    if(naIsRectEmpty(outerrect))
      naError("naIsPosInRect", "Inside test not valid for empty rects.");
    if(!naIsRectValid(outerrect))
      naError("naIsPosInRect", "outerrect is invalid.");
  #endif
  return !((pos.x < outerrect.pos.x)
        || (pos.x > naGetRectEndX(outerrect))
        || (pos.y < outerrect.pos.y)
        || (pos.y > naGetRectEndY(outerrect)));
}
NA_IAPI NABool naIsSizeInSize(NASize size, NASize outersize){
  #ifndef NDEBUG
    if(naIsSizeEmpty(size) || naIsSizeEmpty(outersize))
      naError("naIsSizeInSize", "Inside test not valid for empty sizes.");
    if(!naIsSizeValid(size))
      naError("naIsSizeInSize", "size is invalid.");
    if(!naIsSizeValid(outersize))
      naError("naIsSizeInSize", "outersize is invalid.");
  #endif
  return !((size.width  > outersize.width)
        || (size.height > outersize.height));
}
NA_IAPI NABool naIsRectInRect(NARect rect, NARect outerrect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect) || naIsRectEmpty(outerrect))
      naError("naIsRectInRect", "Inside test not valid for empty rects.");
    if(!naIsRectValid(rect))
      naError("naIsRectInRect", "rect is invalid.");
    if(!naIsRectValid(outerrect))
      naError("naIsRectInRect", "outerrect is invalid.");
  #endif
  return !((rect.pos.x          < outerrect.pos.x)
        || (naGetRectEndX(rect) > naGetRectEndX(outerrect))
        || (rect.pos.y          < outerrect.pos.y)
        || (naGetRectEndY(rect) > naGetRectEndY(outerrect)));
}
NA_IAPI NABool naIsPosiInSizei(NAPosi pos, NASizei outersize){
  #ifndef NDEBUG
    if(!naIsPosiValid(pos))
      naError("naIsPosiInSizei", "pos is invalid.");
    if(naIsSizeiEmpty(outersize))
      naError("naIsPosiInSizei", "Inside test not valid for empty sizes.");
    if(!naIsSizeiValid(outersize))
      naError("naIsPosiInSizei", "outersize is invalid.");
  #endif
  return  ((pos.x >= 0)
        && (pos.x <  outersize.width)
        && (pos.y >= 0)
        && (pos.y <  outersize.height));
}
NA_IAPI NABool naIsPosiInRecti(NAPosi pos, NARecti outerrect){
  #ifndef NDEBUG
    if(!naIsPosiValid(pos))
      naError("naIsPosiInRecti", "pos is invalid.");
    if(naIsRectiEmpty(outerrect))
      naError("naIsPosiInRecti", "Inside test not valid for empty rects.");
    if(!naIsRectiValid(outerrect))
      naError("naIsPosiInRecti", "outerrect is invalid.");
  #endif
  return  ((pos.x >= outerrect.pos.x)
        && (pos.x <  naGetRectiEndX(outerrect))
        && (pos.y >= outerrect.pos.y)
        && (pos.y <  naGetRectiEndY(outerrect)));
}
NA_IAPI NABool naIsSizeiInSizei(NASizei size, NASizei outersize){
  #ifndef NDEBUG
    if(naIsSizeiEmpty(size) || naIsSizeiEmpty(outersize))
      naError("naIsSizeInSize", "Inside test not valid for empty sizes.");
    if(!naIsSizeiValid(size))
      naError("naIsSizeiInSizei", "size is invalid.");
    if(!naIsSizeiValid(outersize))
      naError("naIsSizeiInSizei", "outersize is invalid.");
  #endif
  return !((size.width  > outersize.width)
        || (size.height > outersize.height));
}
NA_IAPI NABool naIsRectiInRecti(NARecti rect, NARecti outerrect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect) || naIsRectiEmpty(outerrect))
      naError("naIsRectiInRecti", "Inside test not valid for empty rects.");
    if(!naIsRectiValid(rect))
      naError("naIsRectiInRecti", "rect is invalid.");
    if(!naIsRectiValid(outerrect))
      naError("naIsRectiInRecti", "outerrect is invalid.");
  #endif
  return  ((rect.pos.x           >= outerrect.pos.x)
        && (naGetRectiEndX(rect) <  naGetRectiEndX(outerrect))
        && (rect.pos.y           >= outerrect.pos.y)
        && (naGetRectiEndY(rect) <  naGetRectiEndY(outerrect)));
}



NA_IAPI double naGetRectEndX (NARect  rect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naGetRectEndX", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naGetRectEndX", "rect is invalid.");
  #endif
  return rect.pos.x + rect.size.width;
}
NA_IAPI double naGetRectEndY (NARect  rect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naGetRectEndY", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naGetRectEndY", "rect is invalid.");
  #endif
  return rect.pos.y + rect.size.height;
}
NA_IAPI NAInt naGetRectiEndX(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiEndX", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiEndX", "rect is invalid.");
  #endif
  return rect.pos.x + rect.size.width;
}
NA_IAPI NAInt naGetRectiEndY(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiEndY", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiEndY", "rect is invalid.");
  #endif
  return rect.pos.y + rect.size.height;
}
NA_IAPI NAInt naGetRectiMaxX(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiMaxX", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiMaxX", "rect is invalid.");
  #endif
  return naGetRectiEndX(rect) - 1;
}
NA_IAPI NAInt naGetRectiMaxY(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiMaxY", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiMaxY", "rect is invalid.");
  #endif
  return naGetRectiEndY(rect) - 1;
}
NA_IAPI NAInt naGetRectiIndexCount(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiMaxY", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiMaxY", "rect is invalid.");
  #endif
  return rect.size.width * rect.size.height;
}


NA_IAPI NAInt naGetRectiIndexOfPosi(NARecti rect, NAPosi pos){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiIndexOfPosi", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiIndexOfPosi", "rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("naGetRectiIndexOfPosi", "pos is invalid.");
    if(!naIsPosiInRecti(pos, rect))
      naError("naGetRectiIndexOfPosi", "pos is not inside rect.");
  #endif
  return (pos.y - rect.pos.y) * rect.size.width + (pos.x - rect.pos.x);
}


NA_IAPI NABool naIsPosValid(NAPos pos){
  return naIsPosFieldValid(pos.x) && naIsPosFieldValid(pos.y);
}
NA_IAPI NABool naIsSizeValid(NASize size){
  return naIsSizeFieldValid(size.width) && naIsSizeFieldValid(size.height);
}
NA_IAPI NABool naIsRectValid(NARect rect){
  return (naIsPosValid(rect.pos) && naIsSizeValid(rect.size));
}
NA_IAPI NABool naIsPosiValid(NAPosi pos){
  return naIsPosiFieldValid(pos.x) && naIsPosiFieldValid(pos.y);
}
NA_IAPI NABool naIsSizeiValid(NASizei size){
  return naIsSizeiFieldValid(size.width) && naIsSizeiFieldValid(size.height);
}
NA_IAPI NABool naIsRectiValid(NARecti rect){
  return (naIsPosiValid(rect.pos) && naIsSizeiValid(rect.size));
}


NA_IAPI NABool naIsSizeEmpty (NASize  size){
  return ((size.width == 0.) || (size.height == 0.));
}
NA_IAPI NABool naIsRectEmpty (NARect  rect){
  return ((rect.size.width == 0.) || (rect.size.height == 0.));
}
NA_IAPI NABool naIsSizeiEmpty(NASizei size){
  return ((size.width == 0) || (size.height == 0));
}
NA_IAPI NABool naIsRectiEmpty(NARecti rect){
  return ((rect.size.width == 0) || (rect.size.height == 0));
}


NA_IAPI NABool naIsSizePositive(NASize size){
  return !((size.width < 0.) || (size.height < 0.));
}
NA_IAPI NABool naIsRectPositive(NARect rect){
  return naIsSizePositive(rect.size);
}
NA_IAPI NABool naIsSizeiPositive(NASizei size){
  return !((size.width < 0) || (size.height < 0));
}
NA_IAPI NABool naIsRectiPositive(NARecti rect){
  return naIsSizeiPositive(rect.size);
}


NA_IAPI NARect naMakeRectPositive(NARect rect){
  NARect newrect;
  if(rect.size.width < 0.){
    newrect.pos.x = rect.pos.x + rect.size.width;
    newrect.size.width = -rect.size.width;
  }else{
    newrect.pos.x = rect.pos.x;
    newrect.size.width = rect.size.width;
  }
  if(rect.size.height < 0.){
    newrect.pos.y = rect.pos.y + rect.size.height;
    newrect.size.height = -rect.size.height;
  }else{
    newrect.pos.y = rect.pos.y;
    newrect.size.height = rect.size.height;
  }
  return newrect;
}
NA_IAPI NARecti naMakeRectiPositive(NARecti rect){
  NARecti newrect;
  if(rect.size.width < 0){
    newrect.pos.x = rect.pos.x + rect.size.width + 1; // important + 1 !
    newrect.size.width = -rect.size.width;
  }else{
    newrect.pos.x = rect.pos.x;
    newrect.size.width = rect.size.width;
  }
  if(rect.size.height < 0){
    newrect.pos.y = rect.pos.y + rect.size.height + 1; // important + 1 !
    newrect.size.height = -rect.size.height;
  }else{
    newrect.pos.y = rect.pos.y;
    newrect.size.height = rect.size.height;
  }
  return newrect;
}




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_COORD_INCLUDED

// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
