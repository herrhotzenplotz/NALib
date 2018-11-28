
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"


// //////////////////////////////////////
// Buffer Source
// //////////////////////////////////////



// Flags for the buffer source:
#define NA_BUFFER_SOURCE_RANGE_LIMITED    0x01
//#define NA_BUFFER_SOURCE_BUFFER           0x02
#define NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE 0x80


// Creates a buffer source with the given descriptor.
NA_HDEF NABufferSource* naCreateBufferSource(NABufferFiller filler, NAMutator simpledestructor){
  NABufferSource* source = naAlloc(NABufferSource);
  naInitRefCount(&(source->refcount));
  source->data = NA_NULL;
  source->datadestructor = NA_NULL;
  source->buffiller = filler;
  source->bufallocator = NA_NULL;
  source->bufdeallocator = NA_NULL;
  source->bufsimpledestructor = simpledestructor;
  source->flags = 0;
  source->limit = naMakeRangeiWithStartAndEnd(0, 0);
  return source;
}



NA_HDEF NABufferSource* naRetainBufferSource(NABufferSource* source){
  #ifndef NDEBUG
    source->flags |= NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE;
  #endif
  return (NABufferSource*)naRetainRefCount(&(source->refcount));
}



NA_HDEF void naDeallocBufferSource(NABufferSource* source){
  if(source->datadestructor){source->datadestructor(source->data);}
  naFree(source);
}



NA_HDEF void naReleaseBufferSource(NABufferSource* source){
  naReleaseRefCount(&(source->refcount), source, source->datadestructor);
}



NA_HDEF void naSetBufferSourceData(NABufferSource* source, void* data, NAMutator datadestructor){
  #ifndef NDEBUG
    if(source->flags & NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE)
      naError("naSetBufferSourceData", "Source already used in a buffer. Mayor problems may occur in the future");
  #endif
  source->data = data;
  source->datadestructor = datadestructor;
}



NA_HDEF void naSetBufferSourceLimit(NABufferSource* source, NARangei limit){
  #ifndef NDEBUG
    if(source->flags & NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE)
      naError("naSetBufferSourceLimit", "Source already used in a buffer. Mayor problems may occur in the future");
  #endif
  source->flags |= NA_BUFFER_SOURCE_RANGE_LIMITED;
  source->limit = limit;
}



NA_HDEF void naSetBufferSourceBufferFunctions(NABufferSource* source, NABufferSourceBufAllocator allocator, NABufferSourceBufDeallocator deallocator){
  #ifndef NDEBUG
    if(source->flags & NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE)
      naError("naSetBufferSourceBufferFunctions", "Source already used in a buffer. Mayor problems may occur in the future");
  #endif
  source->bufallocator = allocator;
  source->bufdeallocator = deallocator;
}



//// NAMutator used in the naNewBufferSourceWithBuffer function.
//NA_HDEF void naDeallocBufferSourceBuffer(NABufferIterator* iter){
//  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);
//  naRelease(buffer);
//  naClearBufferIterator(iter);
//  naFree(iter);
//}



//// This is one of the core concepts of buffer sources: Any NABuffer can again
//// be a source. An iterator retains the desired buffer and manages the access.
//NA_HIDEF NABufferSource* naNewBufferSourceWithBuffer(NABuffer* buffer){
//  NABufferSourceDescriptor desc;
//  NABufferIterator* iter;
//
//  naNulln(&desc, sizeof(desc));
//
//  // We store an iterator on the given buffer as the data. We must allocate
//  // space for it which later will be freed when the pointer is not in use
//  // anymore.
//  iter = naAlloc(NABufferIterator);
//  *iter = naMakeBufferModifier(buffer);
//  naRetain(buffer);
//  desc.data = naNewPointerMutable(iter, (NAMutator)naDeallocBufferSourceBuffer);
//  desc.destructor = (NAMutator)naReleasePointer;
//  desc.flags |= NA_BUFFER_SOURCE_BUFFER;
//  if(naHasBufferFixedRange(buffer)){
//    desc.flags |= NA_BUFFER_SOURCE_RANGE_LIMITED;
//    desc.limit = naGetBufferRange(buffer);
//  }
//
//  // The new buffer source gets created
//  return naNewBufferSource(desc);
//}



#ifndef NDEBUG
  // Returns NA_TRUE if the range is a valid limiting range.
  NA_HDEF NABool naIsBufferSourceLimited(const NABufferSource* source){
    return (source->flags & NA_BUFFER_SOURCE_RANGE_LIMITED);
  }
#endif



// Returns the limit range of this source
NA_HDEF NARangei naGetBufferSourceLimit(NABufferSource* source){
  #ifndef NDEBUG
    if(!naIsBufferSourceLimited(source))
      naError("naGetBufferSourceLimit", "source is not limited");
  #endif
  return source->limit;
}



////// Returns the source data if there is a source and it is defined to be
////// an NABuffer source. returns NA_NULL otherwise.
////NA_HIDEF NABufferIterator* naGetBufferSourceBufferIterator(NABufferSource* source){
////  if(source && (source->desc.flags & NA_BUFFER_SOURCE_BUFFER)){
////    return naGetPointerMutable(source->desc.data);
////  }else{
////    return NA_NULL;
////  }
////}





// Copyright (c) NALib, Tobias Stamm
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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
