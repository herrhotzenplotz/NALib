
// This file contains inline implementations of the file NAArray.h
// Do not include this file directly! It will automatically be included when
// including "NAArray.h"


#include "../NACoord.h"


struct NAArray{
  NAPtr   ptr;         // The pointer storing the data
  size_t   count;       // The number of elements.
  size_t   typeSize;    // The size in bytes of the stored type
  NAMutator destructor;
  #ifndef NDEBUG
    NAInt    iterCount;   // The number of iterators attached to this array.
  #endif
};



NA_IDEF NAArray* naInitArray(NAArray* array){
  #ifndef NDEBUG
    if(!array)
      naCrash("array is Null-Pointer");
  #endif
  array->ptr = naMakePtrWithDataConst(NA_NULL);
  // The typeSize is set to 1 such that calls to naGetArrayCount will not
  // result in bad values.
  array->count = 0;
  array->typeSize = 1;
  array->destructor = NA_NULL;
  #ifndef NDEBUG
    array->iterCount = 0;
  #endif
  return array;
}



NA_IDEF NAArray* naInitArrayWithCount(NAArray* array, size_t typeSize, size_t count){
  #ifndef NDEBUG
    if(!array)
      naCrash("array is Null-Pointer");
    if(typeSize == 0)
      naError("typeSize must not be zero.");
    if(count == 0)
      naError("count must not be zero.");
  #endif
  array->typeSize = typeSize;
  array->count = count;
  array->destructor = (NAMutator)naFree;
  array->ptr = naMakePtrWithDataMutable(naMalloc(count * typeSize));
  #ifndef NDEBUG
    array->iterCount = 0;
  #endif

  return array;
}



NA_IDEF NAArray* naInitArrayWithDataConst(NAArray* array, const void* data, size_t typeSize, size_t count){
  #ifndef NDEBUG
    if(!array)
      naCrash("array is Null-Pointer");
    if(typeSize < 1)
      naError("typeSize should not be < 1");
  #endif
  array->typeSize = typeSize;
  array->count = count;
  array->destructor = NA_NULL;
  array->ptr = naMakePtrWithDataConst(data);
  #ifndef NDEBUG
    array->iterCount = 0;
  #endif

  return array;
}



NA_IDEF NAArray* naInitArrayWithDataMutable(NAArray* array, void* data, size_t typeSize, size_t count, NAMutator destructor){
  #ifndef NDEBUG
    if(!array)
      naCrash("array is Null-Pointer");
    if(typeSize < 1)
      naError("typeSize should not be < 1");
  #endif
  array->typeSize = typeSize;
  array->count = count;
  array->destructor = destructor;
  array->ptr = naMakePtrWithDataMutable(data);
  #ifndef NDEBUG
    array->iterCount = 0;
  #endif

  return array;
}



NA_IDEF void naClearArray(NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
    }
    if(array->iterCount)
      naError("There are still some iterators operating upon this array. Did you use naClearArrayIterator?");
  #endif
  naCleanupPtr(&(array->ptr), array->destructor);
}



NA_IDEF void naForeachArrayConst(NAArray* array, NAAccessor accessor){
  #ifndef NDEBUG
    if(!accessor)
      naCrash("Accessor is Null");
  #endif
  size_t count = naGetArrayCount(array);
  const NAByte* ptr = naGetPtrConst(array->ptr);
  while(count){
    accessor(ptr);
    ptr += array->typeSize;
    count--;
  }
}



NA_IDEF void naForeachArrayMutable(NAArray* array, NAMutator mutator){
  #ifndef NDEBUG
    if(!mutator)
      naCrash("Mutator is Null");
  #endif
  size_t count = naGetArrayCount(array);
  NAByte* ptr = naGetPtrMutable(array->ptr);
  while(count){
    mutator(ptr);
    ptr += array->typeSize;
    count--;
  }
}



NA_IDEF void naForeachArraypConst(NAArray* array, NAAccessor accessor){
  #ifndef NDEBUG
    if(!accessor)
      naCrash("Accessor is Null");
  #endif
  size_t count = naGetArrayCount(array);
  const NAByte* ptr = naGetPtrConst(array->ptr);
  while(count){
    accessor(*((const void**)ptr));
    ptr += array->typeSize;
    count--;
  }
}



NA_IDEF void naForeachArraypMutable(NAArray* array, NAMutator mutator){
  #ifndef NDEBUG
    if(!mutator)
      naCrash("Mutator is Null");
  #endif
  size_t count = naGetArrayCount(array);
  NAByte* ptr = naGetPtrMutable(array->ptr);
  while(count){
    mutator(*((void**)ptr));
    ptr += array->typeSize;
    count--;
  }
}



NA_IDEF const void* naGetArrayPointerConst(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty, returned pointer is Null");
    }
  #endif
  return naGetPtrConst(array->ptr);
}



NA_IDEF void* naGetArrayPointerMutable(NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty, returned pointer is Null");
    }
  #endif
  return naGetPtrMutable(array->ptr);
}



NA_HIDEF size_t na_MakeIndexAbsolute(NAInt index, size_t length){
  if(index < 0){
    index += length;
    #ifndef NDEBUG
      if(index < 0)
        naError("positive index is not positive");
      if((size_t)index >= length)
        naError("positive index overflows length");
    #endif
  }
  return (size_t)index;
}



NA_IDEF const void* naGetArrayElementConst(const NAArray* array, NAInt index){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
  #endif
  size_t absIndex = na_MakeIndexAbsolute(index, naGetArrayCount(array));
  #ifndef NDEBUG
    if(absIndex >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return &(((NAByte*)naGetPtrConst(array->ptr))[absIndex * array->typeSize]);
}



NA_IDEF void* naGetArrayElementMutable(NAArray* array, NAInt index){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
  #endif
  size_t absIndex = na_MakeIndexAbsolute(index, naGetArrayCount(array));
  #ifndef NDEBUG
    if(absIndex >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return &(((NAByte*)naGetPtrMutable(array->ptr))[absIndex * array->typeSize]);
}



NA_IDEF const void* naGetArrayElementpConst(const NAArray* array, NAInt index){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
  #endif
  size_t absIndex = na_MakeIndexAbsolute(index, naGetArrayCount(array));
  #ifndef NDEBUG
    if(absIndex >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return *((const void**)&(((NAByte*)naGetPtrConst(array->ptr))[absIndex * array->typeSize]));
}



NA_IDEF void* naGetArrayElementpMutable(NAArray* array, NAInt index){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
  #endif
  size_t absIndex = na_MakeIndexAbsolute(index, naGetArrayCount(array));
  #ifndef NDEBUG
    if(absIndex >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return *((void**)&(((NAByte*)naGetPtrMutable(array->ptr))[absIndex * array->typeSize]));
}



NA_IDEF size_t naGetArrayCount(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return 0;
    }
    if((NAInt)(array->count) < 0)
      naError("Array count looks like it was negative when interpreted as signed integer. Do not create Arrays with negative sizes!");
  #endif
  // Note that an empty array has a typeSize of 1.
  return array->count;
}



NA_IDEF size_t naGetArrayMaxIndex(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return 0;
    }
    if(naIsArrayEmpty(array))
      naError("array is empty.");
  #endif
  return naMakeMaxWithEnds(naGetArrayCount(array));
}



NA_IDEF size_t naGetArrayTypeSize(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return 0;
    }
  #endif
  return array->typeSize;
}



NA_IDEF NABool naIsArrayEmpty(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return (array->count == 0);
}






// /////////////////////////////////////////
// Array Iterator
// /////////////////////////////////////////


struct NAArrayIterator{
  NAPtr array;
  size_t index;
  #ifndef NDEBUG
    NABool mutator;
  #endif
};



NA_IDEF NAArrayIterator naMakeArrayAccessor(const NAArray* array){
  NAArrayIterator iter;
  #ifndef NDEBUG
    NAArray* mutablearray;
    if(!array)
      naCrash("array is Null pointer");
    mutablearray = (NAArray*)array;
    mutablearray->iterCount++;
    iter.mutator = NA_FALSE;
  #endif
  iter.array = naMakePtrWithDataConst(array);
  iter.index = -1;

  return iter;
}



NA_IDEF NAArrayIterator naMakeArrayMutator(NAArray* array){
  NAArrayIterator iter;
  #ifndef NDEBUG
    NAArray* mutablearray;
    if(!array)
      naCrash("array is Null pointer");
    mutablearray = (NAArray*)array;
    mutablearray->iterCount++;
    iter.mutator = NA_TRUE;
  #endif
  iter.array = naMakePtrWithDataMutable(array);
  iter.index = -1;

  return iter;
}



NA_IDEF void naClearArrayIterator(NAArrayIterator* iterator){
  #ifndef NDEBUG
    NAArray* mutablearray = (NAArray*)naGetPtrConst(iterator->array);
    if(mutablearray->iterCount == 0)
      naError("Too many clears. Iterator count is already zero.");
    mutablearray->iterCount--;
  #else
    NA_UNUSED(iterator);
  #endif
}



NA_IDEF NABool naLocateArrayFirst(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(naIsArrayEmpty(naGetPtrConst(iterator->array)))
      naError("Array is empty");
  #endif
  iterator->index = 0;
  return NA_TRUE;
}



NA_IDEF NABool naLocateArrayLast(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(naIsArrayEmpty(naGetPtrConst(iterator->array)))
      naError("Array is empty");
  #endif
  iterator->index = naGetArrayMaxIndex(naGetPtrConst(iterator->array));
  return NA_TRUE;
}



NA_IDEF NABool naLocateArrayData(NAArrayIterator* iterator, const void* data){
  const NAByte* ptr = naGetArrayPointerConst(naGetPtrConst(iterator->array));
  size_t count = naGetArrayCount(naGetPtrConst(iterator->array));
  size_t index = 0;
  while(count){
    if(ptr == data){
      iterator->index = index;
      return NA_TRUE;
    }
    ptr += naGetArrayTypeSize(naGetPtrConst(iterator->array));
    count--;
    index++;
  }
  return NA_FALSE;
}



NA_IDEF NABool naLocateArrayIndex(NAArrayIterator* iterator, NAInt index){
  #ifndef NDEBUG
    if(naIsArrayEmpty(naGetPtrConst(iterator->array)))
      naError("Array is empty");
  #endif
  size_t absIndex = na_MakeIndexAbsolute(index, naGetArrayCount(naGetPtrConst(iterator->array)));
  iterator->index = absIndex;
  if(absIndex < 0){
    iterator->index = -1;
    return NA_FALSE;
  }else if(absIndex >= naGetArrayCount(naGetPtrConst(iterator->array))){
    iterator->index = -1;
    return NA_FALSE;
  }
  return NA_TRUE;
}



NA_IDEF NABool naIterateArray(NAArrayIterator* iterator, NAInt step){
  if(naIsArrayAtInitial(iterator) && step < 0){iterator->index += naGetArrayCount(naGetPtrConst(iterator->array));}
  iterator->index += step;
  if(iterator->index == naGetArrayCount(naGetPtrConst(iterator->array))){iterator->index = -1;}
  #ifndef NDEBUG
    if(iterator->index < -1)
      naError("Iterator underflows");
    if(iterator->index >= naGetArrayCount(naGetPtrConst(iterator->array)))
      naError("Iterator overflows");
  #endif
  return (iterator->index != -1);
}



NA_IDEF const void* naGetArrayPrevConst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->index == 0){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementConst(naGetPtrConst(iterator->array), -1);
  }else{
    return naGetArrayElementConst(naGetPtrConst(iterator->array), iterator->index - 1);
  }
}



NA_IDEF void* naGetArrayPrevMutable(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->index == 0){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementMutable(naGetPtrMutable(iterator->array), -1);
  }else{
    return naGetArrayElementMutable(naGetPtrMutable(iterator->array), iterator->index - 1);
  }
}



NA_IDEF const void* naGetArrayCurConst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(naIsArrayAtInitial(iterator)){
    return NA_NULL;
  }else{
    return naGetArrayElementConst(naGetPtrConst(iterator->array), iterator->index);
  }
}



NA_IDEF void* naGetArrayCurMutable(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(naIsArrayAtInitial(iterator)){
    return NA_NULL;
  }else{
    return naGetArrayElementMutable(naGetPtrMutable(iterator->array), iterator->index);
  }
}



NA_IDEF const void* naGetArrayNextConst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->index == naGetArrayMaxIndex(naGetPtrConst(iterator->array))){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementConst(naGetPtrConst(iterator->array), 0);
  }else{
    return naGetArrayElementConst(naGetPtrConst(iterator->array), iterator->index + 1);
  }
}



NA_IDEF void* naGetArrayNextMutable(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->index == naGetArrayMaxIndex(naGetPtrConst(iterator->array))){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementMutable(naGetPtrMutable(iterator->array), 0);
  }else{
    return naGetArrayElementMutable(naGetPtrMutable(iterator->array), iterator->index + 1);
  }
}



NA_IDEF NABool naIsArrayAtFirst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  return (iterator->index == 0);
}



NA_IDEF NABool naIsArrayAtLast(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  return (iterator->index == naGetArrayMaxIndex(naGetPtrConst(iterator->array)));
}



NA_IDEF NABool naIsArrayAtIndex(const NAArrayIterator* iterator, NAInt index){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  index = na_MakeIndexAbsolute(index, naGetArrayCount(naGetPtrConst(iterator->array)));
  return (iterator->index == index);
}



NA_IDEF NABool naIsArrayAtInitial(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  return (iterator->index == -1);
}



NA_IDEF size_t naGetArrayCurIndex(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_INVALID_MEMORY_INDEX;
    }
    if(naIsArrayAtInitial(iterator))
      naError("iterator is at initial position.");
  #endif
  return iterator->index;
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
