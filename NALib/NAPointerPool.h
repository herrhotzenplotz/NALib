
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_POINTERPOOL_INCLUDED
#define NA_POINTERPOOL_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NASystem.h"



typedef struct NAPointerPool NAPointerPool;


struct NAPointerPool{
  void* data[NA_SYSTEM_ADDRESS_BITS];
  void** pointer[NA_SYSTEM_ADDRESS_BITS];
  NAUInt usedcount[NA_SYSTEM_ADDRESS_BITS];
  NAUInt arraycount;
};


void naExpandPointerPool(NAPointerPool* pointerpool){
  NAUInt subcount = 1 << pointerpool->arraycount;
  pointerpool->data[pointerpool->arraycount] = naMalloc(typesize * subcount);
  pointerpool->pointer[pointerpool->arraycount] = naMalloc(subcount * sizeof(void*));
  pointerpool->usedcount[pointerpool->arraycount] = 0;
  NAByte* dataptr = pointerpool->data[pointerpool->arraycount];
  void** pointerptr = pointerpool->pointer[pointerpool->arraycount];
  for(NAUInt i=0; i<subcount; i++){
    *pointerptr = dataptr;
    pointerptr++;
    dataptr += typesize;
  }
  pointerpool->arraycount++;
}


void naShrinkPointerPool(NAPointerPool* pointerpool){
  #ifndef NDEBUG
    if(pointerpool->arraycount == 0)
      naError("naShrinkPointerPool", "Pool is empty");
  #endif
  pointerpool->arraycount--;
  #ifndef NDEBUG
    if(pointerpool->usedcount[pointerpool->arraycount] != 0)
      naError("naShrinkPointerPool", "Subarray contains elements still in use");
  #endif
}


NAPointerPool* naCreatePointerPool(NAPointerPool* pointerpool){
  pointerpool->usedcount = 0;
  pointerpool->arraycount = 0;
  return pointerpool;
}


NAUInt naGetPointerPoolAllocatedCount(NAPointerPool* pointerpool){
  return (1 << pointerpool->arraycount) - 1;
}

#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_POINTERPOOL_INCLUDED


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
