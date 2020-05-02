
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../testNALib.h"


const char* na_signed_integer_encoding_enum_strings[] = {
  "Unknown",
  "Sign & Magnitude",
  "Ones complement",
  "Twos complement"
};

const char* na_native_types_enum_strings[] = {
  "No native type",
  "native char type",
  "native short type",
  "native int type",
  "native long type",
  "native long long type",
  "native int128 type",
  "native int256 type"
};

void printNAEncoding(){
  printf("NAEncoding.h fixed values:" NA_NL);
  printMacroInt          (NA_SIGN_ENCODING_UNKNOWN);
  printMacroInt          (NA_SIGN_ENCODING_SIGN_MAGNITUDE);
  printMacroInt          (NA_SIGN_ENCODING_ONES_COMPLEMENT);
  printMacroInt          (NA_SIGN_ENCODING_TWOS_COMPLEMENT);

  printMacroInt          (NA_TYPE_NATIVE_NONE);
  printMacroInt          (NA_TYPE_NATIVE_CHAR);
  printMacroInt          (NA_TYPE_NATIVE_SHORT);
  printMacroInt          (NA_TYPE_NATIVE_INT);
  printMacroInt          (NA_TYPE_NATIVE_LONG);
  printMacroInt          (NA_TYPE_NATIVE_LONG_LONG);
  printMacroInt          (NA_TYPE_NATIVE_INT_128);
  printMacroInt          (NA_TYPE_NATIVE_INT_256);

  printf(NA_NL "NAEncoding.h computed values:" NA_NL);
  printMacroEnum         (NA_SIGN_ENCODING, na_signed_integer_encoding_enum_strings, 4);
  printMacroInt          (NA_TYPE_NATIVE_CHAR_BITS);
  printMacroInt          (NA_TYPE_NATIVE_SHORT_BITS);
  printMacroInt          (NA_TYPE_NATIVE_INT_BITS);
  printMacroInt          (NA_TYPE_NATIVE_LONG_BITS);
  printMacroInt          (NA_TYPE_NATIVE_LONG_LONG_BITS);
  printMacroEnum         (NA_TYPE_INT8_REPRESENTATION, na_native_types_enum_strings, 8);
  printMacroEnum         (NA_TYPE_INT16_REPRESENTATION, na_native_types_enum_strings, 8);
  printMacroEnum         (NA_TYPE_INT32_REPRESENTATION, na_native_types_enum_strings, 8);
  printMacroEnum         (NA_TYPE_INT64_REPRESENTATION, na_native_types_enum_strings, 8);
  printMacroEnum         (NA_TYPE_INT128_REPRESENTATION, na_native_types_enum_strings, 8);
  printMacroEnum         (NA_TYPE_INT256_REPRESENTATION, na_native_types_enum_strings, 8);
  printMacroPlain        (NA_TYPE_INT8);
  printMacroPlain        (NA_TYPE_INT16);
  printMacroPlain        (NA_TYPE_INT32);
  printMacroPlain        (NA_TYPE_INT64);
  printMacroPlain        (NA_TYPE_INT128);
  printMacroPlain        (NA_TYPE_INT256);

  printf(NA_NL);
}



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

