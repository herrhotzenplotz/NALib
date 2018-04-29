

//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//

#include "NABuffer.h"
#include "NAFile.h"
#include "NADateTime.h"
#include "NABinaryOperators.h"

#include "../NALib/NABase.h"
#include <stdio.h>

int main(void){
  printf("NALib Version: %d ", NA_VERSION);
  #ifndef NDEBUG
    printf("(Debug ");
  #else
    printf("(Release ");
  #endif
  printf("%d Bits Addresses, %d Bits Integers)\n", NA_SYSTEM_ADDRESS_BITS, NA_TYPE_NAINT_BITS);

  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
   NA_UNUSED(getchar());
  #endif
  
  naStartRuntime();

  naMakeDateTimeNow();
  printf("%d\n", NA_TYPE_NATIVE_CHAR_BITS);
  printf("%d\n", NA_TYPE_NATIVE_SHORT_INT_BITS);
  printf("%d\n", NA_TYPE_NATIVE_INT_BITS);
  printf("%d\n", NA_TYPE_NATIVE_LONG_INT_BITS);
  #if defined NA_TYPE_NATIVE_LONG_LONG_INT_BITS
    printf("%d\n", NA_TYPE_NATIVE_LONG_LONG_INT_BITS);
  #else
    printf("No long long available\n");
  #endif

  printf("%d\n", NA_TYPE_INT8);
  printf("%d\n", NA_TYPE_INT16);
  printf("%d\n", NA_TYPE_INT32);
  #if defined NA_TYPE_INT64
    printf("%d\n", NA_TYPE_INT64);
  #else
    printf("No int64 available\n");
  #endif

//  printf("%d\n", NA_IEEE754_SINGLE_BIT_COUNT);
//  printf("%d\n", NA_IEEE754_SINGLE_SIGNIFICAND_BIT_COUNT);
//  printf("%08" NA_PRIx32 "\n", NA_IEEE754_SINGLE_SIGNIFICAND_NORM);
//  printf("%08" NA_PRIx32 "\n", NA_IEEE754_SINGLE_SIGNIFICAND_MASK);
//  printf("%d\n", NA_IEEE754_SINGLE_EXPONENT_BIT_COUNT);
//  printf("%d\n", NA_IEEE754_SINGLE_EXPONENT_BIAS);
//  printf("%d\n", NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL);
//  printf("%d\n", NA_IEEE754_SINGLE_EXPONENT_SPECIAL);
//  printf("%08" NA_PRIx32 "\n", NA_IEEE754_SINGLE_EXPONENT_MASK);
//  printf("%08" NA_PRIx32 "\n", NA_IEEE754_SINGLE_SIGN_MASK);
//
//printf("\n");
//
//  printf("%d\n", NA_IEEE754_DOUBLE_BIT_COUNT);
//  printf("%d\n", NA_IEEE754_DOUBLE_SIGNIFICAND_BIT_COUNT);
//  printf("%016" NA_PRIx64 "\n", NA_IEEE754_DOUBLE_SIGNIFICAND_NORM);
//  printf("%016" NA_PRIx64 "\n", NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
//  printf("%d\n", NA_IEEE754_DOUBLE_EXPONENT_BIT_COUNT);
//  printf("%d\n", NA_IEEE754_DOUBLE_EXPONENT_BIAS);
//  printf("%d\n", NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL);
//  printf("%d\n", NA_IEEE754_DOUBLE_EXPONENT_SPECIAL);
//  printf("%016" NA_PRIx64 "\n", NA_IEEE754_DOUBLE_EXPONENT_MASK);
//  printf("%016" NA_PRIx64 "\n", NA_IEEE754_DOUBLE_SIGN_MASK);
//  
//  float testfloat = naCreateFloat(1, NA_IEEE754_SINGLE_EXPONENT_SPECIAL);
//  printf("%e\n", testfloat);
//  double testdouble = naCreateDoubleWithExponent(NA_IEEE754_DOUBLE_EXPONENT_SPECIAL-1);
//  printf("%e\n", testdouble);
//  double testdouble = naCreateDoubleSubnormal(NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
//  printf("%e\n", testdouble);
//  double testdouble = naPowerOf2d(4);
//  printf("%f\n", testdouble);
  
//  NABuffer* buf = naNewBufferWithInpuFile("test.txt");
//  NABufferIterator iter = naMakeBufferAccessor(buf);
//  naSeekBufferFromStart(&iter, 0);
//  printf("%d\n", naParseBufferInt32(&iter, NA_FALSE));
//  naClearBufferIterator(&iter);
//  naRelease(buf);
  
  naStopRuntime();
  
  return 0;
}

