
#include "NATest.h"
#include "NABase.h"
#include <stdio.h>



void na_AccessorFunc(const void* param){
  NA_UNUSED(param);
}
void na_MutatorFunc(void* param){
  NA_UNUSED(param);
}



void testNALanguage(void){
  naTestGroup("typedefs"){
    NAAccessor testAccessor = na_AccessorFunc;
    NAMutator testMutator = na_MutatorFunc;
    naTest(testAccessor == na_AccessorFunc);
    naTest(testMutator == na_MutatorFunc);
  }
}



void printNALanguage(void){
  printf("NALanguage.h:" NA_NL);
  
  naPrintMacro(NA_RESTRICT);
  naPrintMacro(NA_INLINE);
  naPrintMacro(NA_LINKER_NO_EXPORT);
  naPrintMacro(NA_LINKER_EXPORT);

  naPrintMacro(NA_API);
  naPrintMacro(NA_DEF);
  naPrintMacro(NA_IAPI);
  naPrintMacro(NA_IDEF);
  naPrintMacro(NA_HAPI);
  naPrintMacro(NA_HDEF);
  naPrintMacro(NA_HIAPI);
  naPrintMacro(NA_HIDEF);

  naPrintMacro(NA_NULL);
  naPrintMacro(NA_NORETURN);
  naPrintMacro(NA_UNUSED(parameter));

  printf(NA_NL);
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
