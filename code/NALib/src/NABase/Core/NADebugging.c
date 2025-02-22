
#include "../../NABase.h"

#if NA_DEBUG

  #if NA_TESTING_ENABLED == 1
    #include "NATest.h"
  #endif

  #include <stdio.h>    // for the fprintf function



  // The error printing method. Errors will be emitted to the stderr output.
  // When NA_DEBUG is 0, this function is OBSOLETE!

  NA_HDEF void na_Error(const char* functionSymbol, const char* text){
    NABool doPrintOut = NA_TRUE;
    #if NA_TESTING_ENABLED == 1
      doPrintOut = !naIsTestCaseRunning();
      naIncErrorCount();
    #endif

    if(doPrintOut){
      fprintf(stderr, "Error in %s: %s", functionSymbol, text);

      // //////////////////////////
      // Set a breakpoint in the following line to debug.
      // //////////////////////////

      fprintf(stderr, NA_NL);

      // //////////////////////////
    }
  }

  
  
  NA_HDEF void na_Crash(const char* functionSymbol, const char* text){
    #if NA_TESTING_ENABLED == 1
      // Disable the testcase, such that a call to na_Error actually produces
      // an output.
      naSetTestCaseRunning(NA_FALSE);
    #endif
    na_Error(functionSymbol, text);
    fprintf(stderr, NA_NL "Stopping the application deliberately." NA_NL);
}



#endif // NA_DEBUG



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
