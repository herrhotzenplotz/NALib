
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_GROWING_SPACE_INCLUDED
#define NA_GROWING_SPACE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This data structure provides the functionality of storing multiple elements
// of the same type but with a yet unknown count. The number of elements
// stored is only limited by the available memory.
//
// Usage: Create a growing space and add each element one by one. When you
// are finished adding elements, dump everything into a desired structure
// like an array or a list. Or just leave the elements in this structure and
// delete them all together when you are finished using them. Any other usage
// is not encouraged.
//
// Implementation note:
// NAGrowingSpace stores elements in a list of arrays. Therefore, the addresses
// of your elements will not change as long as they are stored in the growing
// space. Traversing the space one-by-one can be done efficiently using an
// iterator. You can even get pointers to the arrays, but only const.
//
// The growing space grows exponentially. Starting with an initial count, the
// following arrays will always be double in size. Choose your initial size
// wisely.
//
// IMPORTANT:
// NAGrowingSpace is not a Pool structure! Deletion or recycling of elements
// is not provided. NAGrowingSpace is a strictly growing structure which is
// especially good at providing quick space for single elements which
// simply need to be stored somewhere where the programmer does not want to
// care about.
//
// Use this structure when you are building up collections, for example:
// - parsing files without prior knowledge of the number of elements stored.
// - Creating a bunch of particles in a physically based simulation.
// - Creating an unknown number of edges within a point cloud.
// And many more examples. Turns out the author uses this structure more and
// more.

#include "NAArray.h"
#include "NAList.h"


// Opaque type. See explanation in readme.txt
typedef struct NAGrowingSpace NAGrowingSpace;


// Creates a new NAGrowingSpace with the desired type size. The space will grow
// exponentially over time. With the minimalcount parameter, you can define,
// where the exponential growth shall start. The minimalcount parameter must
// be a power of 2 and at least 2. The values 0 and 1 are not allowed.
NA_IAPI NAGrowingSpace* naInitGrowingSpace(NAGrowingSpace* space,
                                                     NAUInt typesize,
                                                     NAUInt minimalcount);

// Clears the given space.
NA_IAPI void naClearGrowingSpace  (NAGrowingSpace* space);

// Grows the space by 1 element. The pointer to the new element is returned.
// The returned pointer points to an uninitialized space.
NA_IAPI void* naNewGrowingSpaceElement(NAGrowingSpace* space);

// Iteration functions
//
// Every growing space has an internal pointer denoting the current element.
// You can directly access the content of that element. If no current element
// is set, NA_NULL is returned.
NA_IAPI const void* naGetGrowingSpaceConstContent  (const NAGrowingSpace* space);
NA_IAPI       void* naGetGrowingSpaceMutableContent(const NAGrowingSpace* space);
// Returns a pointer to the current array the iterator points to and returns
// the number of elements in that array in count
NA_IAPI const void* naGetGrowingSpaceArrayContent  (const NAGrowingSpace* space,
                                                                 NAUInt* count);

// The following functions move the internal pointer. At start, the internal
// pointer is not set.
NA_IAPI void naFirstGrowingSpace(const NAGrowingSpace* space);
NA_IAPI void naNextGrowingSpaceElement(const NAGrowingSpace* space);
// Moves the internal pointer to the start of the next internal array.
NA_IAPI void naNextGrowingSpaceArray(const NAGrowingSpace* space);

// Returns the number of elements actually stored in the space
NA_IAPI NAUInt naGetGrowingSpaceCount(const NAGrowingSpace* space);









// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


struct NAGrowingSpace{
  NAList        arrays;         // List of void*
  NAUInt        typesize;       // The size in bytes of the stored type
  NAUInt        usedcount;      // The used number of elements in the storage.
  NAUInt        minimalexp;     // The exponent of 2 denoting the minimal count.
  NAUInt        curarraycount;  // The element count of the current array.
  NAUInt        cur;            // The index in the current array if available.
};


// This is a helper function which has one purpose: Add another memory block
// to the space which is double the size of the previous block. The first block
// has the size given to naInitGrowingSpace
NA_HIDEF void naAddGrowingSpaceNewSpace(NAGrowingSpace* space){
  NAUInt newelementcount;
  NAUInt arraycount = naGetListCount(&(space->arrays));
  newelementcount = 1 << (arraycount + space->minimalexp);
  void* newarray = naMalloc(newelementcount * space->typesize);
  naAddListLastMutable(&(space->arrays), newarray);
}



NA_IDEF NAGrowingSpace* naInitGrowingSpace(NAGrowingSpace* space, NAUInt typesize, NAUInt minimalcount){
  #ifndef NDEBUG
    if(!space)
      {naCrash("naInitGrowingSpace", "space is NULL"); return NA_NULL;}
    if(typesize <= 0)
      naError("naInitGrowingSpace", "typesize is invalid.");
    // The following test states that 0 and 1 are invalid values. They could be
    // automatically converted to 2 but they are reserved for future use.
    if(minimalcount < 2)
      naError("naInitGrowingSpace", "minimalcount must be at least 2");
  #endif
  naInitList(&(space->arrays));
  space->typesize = typesize;
  space->usedcount = 0;
  space->minimalexp = naLog2i(minimalcount);
  #ifndef NDEBUG
    if((NAUInt)naPow(2, space->minimalexp) != minimalcount)
      naError("naInitGrowingSpace", "minimalcount must be a power of 2.");
  #endif
  if(space->minimalexp == 0){space->minimalexp = 1;}
  return space;
}



NA_IDEF void naClearGrowingSpace(NAGrowingSpace* space){
  void* curarray; // Declaration before Implementation.
  #ifndef NDEBUG
    if(!space){
      naCrash("naClearGrowingSpace", "space is Null-Pointer.");
      return;
    }
  #endif

  naFirstList(&(space->arrays));
  while((curarray = naIterateListMutable(&(space->arrays), 1))){
    naFree(curarray);
  }

  naClearList(&(space->arrays), NA_NULL);
}



NA_IDEF void* naNewGrowingSpaceElement(NAGrowingSpace* space){
  // Declaration before Implementation. Needed for C90
  NAUInt subindex;
  NAUInt baseindex;

  space->usedcount++;
  NAUInt arraycount = naGetListCount(&(space->arrays));
  
  NAUInt availablespace = (1 << (arraycount + space->minimalexp)) - (1 << space->minimalexp);

  if(space->usedcount > availablespace){
    naAddGrowingSpaceNewSpace(space);
    baseindex = (1 << (arraycount + space->minimalexp)) - (1 << space->minimalexp);
  }else{
    baseindex = (1 << (arraycount + space->minimalexp - 1)) - (1 << space->minimalexp);
  }
  
  subindex = space->usedcount - 1 - baseindex;
  return &(((NAByte*)naGetListLastMutable(&(space->arrays)))[subindex * space->typesize]);
}



NA_IDEF const void* naGetGrowingSpaceConstContent(const NAGrowingSpace* space){
  void* curarray = naGetListCurrentMutable(&(space->arrays));
  if(!curarray){return NA_NULL;}
  return &(((const NAByte*)curarray)[space->cur * space->typesize]);
}



NA_IDEF void* naGetGrowingSpaceMutableContent(const NAGrowingSpace* space){
  if(naGetListCount(&(space->arrays)) == NA_ZERO){return NA_NULL;}
  void* curarray = naGetListCurrentMutable(&(space->arrays));
  if(!curarray){return NA_NULL;}
  return &(((NAByte*)curarray)[space->cur * space->typesize]);
}



NA_IDEF const void* naGetGrowingSpaceArrayContent(const NAGrowingSpace* space, NAUInt* count){
  const void* curarray = naGetListCurrentConst(&(space->arrays));
  if(!curarray){return NA_NULL;}
  *count = space->curarraycount;
  return curarray;
}



NA_HIDEF void naCorrectGrowingSpaceCurArraySize(NAGrowingSpace* space){
  if(naIsListAtLast(&(space->arrays))){
    NAUInt arraycount = naGetListCount(&(space->arrays));
    NAUInt baseindex;
    // This line is the reason why minimalcount shall not be 0 or 1. In that
    // case, the following << operator would have a negative input value which
    // is undefined.
    baseindex = (1 << (arraycount + space->minimalexp - 1)) - (1 << space->minimalexp);
    space->curarraycount = space->usedcount - baseindex;
  }
}


NA_HIDEF void naAdvanceGrowingSpaceToNextArray(NAGrowingSpace* space){
  #ifndef NDEBUG
    if(naIsListEmpty(&(space->arrays)))
      naError("naAdvanceGrowingSpaceToNextArray", "Growing space is empty");
  #endif
  naIterateList(&(space->arrays), 1);
  space->cur = 0;
  space->curarraycount <<= 1;
  naCorrectGrowingSpaceCurArraySize(space);
}



NA_IDEF void naFirstGrowingSpace(const NAGrowingSpace* space){
  NAGrowingSpace* mutablespace = (NAGrowingSpace*)space;
  naFirstList(&(mutablespace->arrays));
  mutablespace->cur = 0;
  mutablespace->curarraycount = 1 << space->minimalexp;
  // Note that this is not correct if the growing space is empty. But this
  // will be captured in accessing functions by checking if there is anything
  // inside the list at all.
  naCorrectGrowingSpaceCurArraySize(mutablespace);
}



NA_IDEF void naNextGrowingSpaceElement(const NAGrowingSpace* space){
  NAGrowingSpace* mutablespace = (NAGrowingSpace*)space;
  const void* curarray = naGetListCurrentConst(&(space->arrays));
  if(!curarray){return;}
  
  mutablespace->cur++;
  if(mutablespace->cur == mutablespace->curarraycount){
    // We go outside the current array.
    naAdvanceGrowingSpaceToNextArray(mutablespace);
  }
}



NA_IDEF void naNextGrowingSpaceArray(const NAGrowingSpace* space){
  NAGrowingSpace* mutablespace = (NAGrowingSpace*)space;
  naAdvanceGrowingSpaceToNextArray(mutablespace);
}




NA_IDEF NAUInt naGetGrowingSpaceCount(const NAGrowingSpace* space){
  return space->usedcount;
}






#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_GROWING_SPACE_INCLUDED



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
