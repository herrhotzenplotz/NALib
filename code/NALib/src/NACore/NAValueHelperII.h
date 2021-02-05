
// This file contains inline implementations of the file NAValueHelper.h
// Do not include this file directly! It will automatically be included when
// including "NAValueHelper.h"



NA_IDEF NABool naGetFlag(int flags, int flag){
  return ((flags & flag) == flag);
}
NA_IDEF NABool naGetFlagi(NAInt flags, NAInt flag){
  return ((flags & flag) == flag);
}
NA_IDEF NABool naGetFlagu(NAUInt flags, NAUInt flag){
  return ((flags & flag) == flag);
}
NA_IDEF void naSetFlag(int* flags, int flag, NABool set){
  #ifndef NDEBUG
    if(flags == NA_NULL)
      naCrash("flags is Nullpointer");
    if(set != NA_FALSE && set != NA_TRUE)
      naError("Boolean value invalid");
  #endif
  *flags = (*flags & ~flag) | (set * flag);
}
NA_IDEF void naSetFlagi(NAInt* flags, NAInt flag, NABool set){
  #ifndef NDEBUG
    if(flags == NA_NULL)
      naCrash("flags is Nullpointer");
    if(set != NA_FALSE && set != NA_TRUE)
      naError("Boolean value invalid");
  #endif
  *flags = (*flags & ~flag) | (set * flag);
}
NA_IDEF void naSetFlagu(NAUInt* flags, NAUInt flag, NABool set){
  #ifndef NDEBUG
    if(flags == NA_NULL)
      naCrash("flags is Nullpointer");
    if(set != NA_FALSE && set != NA_TRUE)
      naError("Boolean value invalid");
  #endif
  *flags = (*flags & ~flag) | ((NAUInt)set * flag);
}
NA_IDEF NABool naToggleFlag(int* flags, int flag){
  #ifndef NDEBUG
    if(flags == NA_NULL)
      naCrash("flags is Nullpointer");
  #endif
  (*flags) ^= flag;
  return naGetFlag(*flags, flag);
}
NA_IDEF NABool naToggleFlagi(NAInt* flags, NAInt flag){
  #ifndef NDEBUG
    if(flags == NA_NULL)
      naCrash("flags is Nullpointer");
  #endif
  (*flags) ^= flag;
  return naGetFlagi(*flags, flag);
}
NA_IDEF NABool naToggleFlagu(NAUInt* flags, NAUInt flag){
  #ifndef NDEBUG
    if(flags == NA_NULL)
      naCrash("flags is Nullpointer");
  #endif
  (*flags) ^= flag;
  return naGetFlagu(*flags, flag);
}



NA_IDEF int32 naMakeMaxWithEndi32(int32 end){
  #ifndef NDEBUG
    if(end == NA_MIN_i32)
      naError("Integer underflow");
  #endif
  return end - NA_ONE_i32;
}
NA_IDEF NAi64 naMakeMaxWithEndi64(NAi64 end){
  #ifndef NDEBUG
    if(naEquali64(end, NA_MIN_i64))
      naError("Integer underflow");
  #endif
  return naSubi64(end, NA_ONE_i64);
}
NA_IDEF NAInt naMakeMaxWithEndi(NAInt end){
  #if NA_TYPE_NAINT_BITS == 32
    return naMakeMaxWithEndi32(end);
  #elif NA_TYPE_NAINT_BITS == 64
    return naMakeMaxWithEndi64(end);
  #endif
}

NA_IDEF int32 naMakeEndWithMaxi32(int32 max){
  #ifndef NDEBUG
    if(max == NA_MAX_i32)
      naError("Integer overflow");
  #endif
  return max + NA_ONE_i32;
}
NA_IDEF NAi64 naMakeEndWithMaxi64(NAi64 max){
  #ifndef NDEBUG
    if(naEquali64(max, NA_MAX_i64))
      naError("Integer overflow");
  #endif
  return naAddi64(max, NA_ONE_i64);
}
NA_IDEF NAInt naMakeEndWithMaxi(NAInt max){
  #if NA_TYPE_NAINT_BITS == 32
    return naMakeEndWithMaxi32(max);
  #elif NA_TYPE_NAINT_BITS == 64
    return naMakeEndWithMaxi64(max);
  #endif
}

NA_IDEF double naMakeEndWithStartAndLength(double start, double length){
  double result = start + length;
  #ifndef NDEBUG
    if(!naIsOffsetValueValid(result)){
      naError("result invalid");
    }
  #endif
  return result;
}
NA_IDEF float naMakeEndWithStartAndLengthf(float start, float length){
  float result = start + length;
  #ifndef NDEBUG
    if(!naIsOffsetValueValidf(result)){
      naError("result invalid");
    }
  #endif
  return result;
}
NA_IDEF NAInt naMakeEndWithStartAndLengthi(NAInt start, NAInt length){
  NAInt result = start + length;
  #ifndef NDEBUG
    if(length > 0){
      if(result < start)
        naError("Integer overflow");
    }else{
      if(result > start)
        naError("Integer underflow");
    }
  #endif
  return result;
}

NA_IDEF NAInt naMakeMaxWithMinAndLengthi(NAInt min, NAInt length){
  NAInt result = naMakeMaxWithEndi(min + length);
  #ifndef NDEBUG
    if(length > 0){
      if(result < min)
        naError("Integer overflow");
    }else{
      if(result > min)
        naError("Integer underflow");
    }
  #endif
  return result;
}

NA_IDEF double naMakeLengthWithStartAndEnd(double start, double end){
  double result = end - start;
  #ifndef NDEBUG
  if(start > end)
    naError("start is greater than end");
  if(!naIsLengthValueValid(result)){
    naError("result invalid");
  }
  #endif
  return result;
}
NA_IDEF float naMakeLengthWithStartAndEndf(float start, float end){
  float result = end - start;
  #ifndef NDEBUG
  if(start > end)
    naError("start is greater than end");
  if(!naIsLengthValueValidf(result)){
    naError("result invalid");
  }
  #endif
  return result;
}
NA_IDEF NAInt naMakeLengthWithStartAndEndi(NAInt start, NAInt end){
  NAInt result = end - start;
  #ifndef NDEBUG
  if(start > end)
    naError("start is greater than end");
  if(end > start){
    if(result < 0)
      naError("Integer underflow");
  }else{
    if(result > 0)
      naError("Integer overflow");
  }
  #endif
  return result;
}
NA_IDEF NAInt naMakeLengthWithMinAndMaxi(NAInt min, NAInt max){
  #ifndef NDEBUG
  if(min > max + 1)
    naError("min is greater than max + 1");
  #endif
  return naMakeEndWithMaxi(max) - min;
}



#include "../NAMathOperators.h"

NA_IDEF NAInt naAlignValuei(NAInt x, NAInt offset, NAInt alignlength){
  NAInt shiftx;
  #ifndef NDEBUG
  if(!naIsLengthValueUsefuli(alignlength))
    naError("Invalid size leads to range overflow. Correcting to empty range.");
  #endif
  shiftx = x - offset;
  if(shiftx < 0){
    return (((NAInt)((shiftx + 1) / alignlength) - 1) * alignlength) + offset;
  }else{
    return (((NAInt)((shiftx + 0) / alignlength) - 0) * alignlength) + offset;
  }
}
NA_IDEF double naAlignValued(double x, double offset, double alignlength){
  double shiftx;
  #ifndef NDEBUG
  if(!naIsLengthValueUseful(alignlength))
    naError("Invalid size leads to range overflow. Correcting to empty range.");
  #endif
  shiftx = x - offset;
  return ((naFloor((shiftx) / alignlength)) * alignlength) + offset;
}



NA_IDEF NAInt naMakeIntWithIntegerFloat(float x){
  #ifndef NDEBUG
    if(naRoundf(x) != x)
      naError("Given float is not an integer number");
  #endif
  return (NAInt)x;
}
NA_IDEF NAInt naMakeIntWithIntegerDouble(double x){
  #ifndef NDEBUG
    if(naRound(x) != x)
      naError("Given double is not an integer number");
  #endif
  return (NAInt)x;
}



NA_IDEF NABool naIsOffsetValueValid(double a){
  return !naIsNaN(a);
}
NA_IDEF NABool naIsOffsetValueValidf(float a){
  return !naIsNaNf(a);
}
NA_IDEF NABool naIsOffsetValueValidi(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}
NA_IDEF NABool naIsOffsetValueValidu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_TRUE;
}

NA_IDEF NABool naIsLengthValueValid(double a){
  return !naIsNaN(a);
}
NA_IDEF NABool naIsLengthValueValidf(float a){
  return !naIsNaNf(a);
}
NA_IDEF NABool naIsLengthValueValidi(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}
NA_IDEF NABool naIsLengthValueValidu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_TRUE;
}

NA_IDEF NABool naIsLengthValueEmpty(double a){
  return (a == 0.);
}
NA_IDEF NABool naIsLengthValueEmptyf(float a){
  return (a == 0.f);
}
NA_IDEF NABool naIsLengthValueEmptyi(NAInt a){
  return (a == 0);
}
NA_IDEF NABool naIsLengthValueEmptyu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #endif
  return (a == 0);
}


NA_IDEF NABool naIsLengthValueNegative(double a){
  return (a < 0.);
}
NA_IDEF NABool naIsLengthValueNegativef(float a){
  return (a < 0.f);
}
NA_IDEF NABool naIsLengthValueNegativei(NAInt a){
  return (a < 0);
}
NA_IDEF NABool naIsLengthValueNegativeu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_FALSE;
}


NA_IDEF NABool naIsOffsetValueUseful(double a){
  return !naIsNaN(a);
}
NA_IDEF NABool naIsOffsetValueUsefulf(float a){
  return !naIsNaNf(a);
}
NA_IDEF NABool naIsOffsetValueUsefuli(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}
NA_IDEF NABool naIsOffsetValueUsefulu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_TRUE;
}


NA_IDEF NABool naIsLengthValueUseful(double a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_IDEF NABool naIsLengthValueUsefulf(float a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_IDEF NABool naIsLengthValueUsefuli(NAInt a){
  return (a > 0);
}
NA_IDEF NABool naIsLengthValueUsefulu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #endif
  return (a > 0);
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
