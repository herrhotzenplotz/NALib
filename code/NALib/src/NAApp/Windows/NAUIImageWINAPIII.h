
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


#include "../../NAVisual/NAPNG.h"
#include "../../NAApp.h"
#include <wincodec.h>


NA_DEF NAUIImageSkin naGetSkinForCurrentAppearance(){
  return NA_UIIMAGE_SKIN_LIGHT;
}


NA_DEF void naFillDefaultTextColorWithSkin(NABabyColor color, NAUIImageSkin skin){
  uint8 skinColor[4];
  switch(skin){
  case NA_UIIMAGE_SKIN_LIGHT:
    skinColor[0] = 16;
    skinColor[1] = 16;
    skinColor[2] = 16;
    skinColor[3] = 255;
    break;
  case NA_UIIMAGE_SKIN_DARK:
    skinColor[0] = 240;
    skinColor[1] = 240;
    skinColor[2] = 240;
    skinColor[3] = 255;
    break;
  default:
    #if NA_DEBUG
      naError("Cannot provide color for plain skin");
    #endif
    break;
  }
  naFillBabyColorWithu8(color, skinColor, NA_COLOR_BUFFER_RGBA);
}



NA_DEF void naFillDefaultLinkColorWithSkin(NABabyColor color, NAUIImageSkin skin){
  uint8 skinColor[4];
  switch(skin){
  case NA_UIIMAGE_SKIN_LIGHT:
  skinColor[0] = 16;
  skinColor[1] = 128;
  skinColor[2] = 240;
  skinColor[3] = 255;
  break;
  case NA_UIIMAGE_SKIN_DARK:
  skinColor[0] = 64;
  skinColor[1] = 196;
  skinColor[2] = 240;
  skinColor[3] = 255;
  break;
  default:
  #if NA_DEBUG
  naError("Cannot provide color for plain skin");
  #endif
  break;
  }
  naFillBabyColorWithu8(color, skinColor, NA_COLOR_BUFFER_RGBA);
}



NA_DEF void naFillDefaultAccentColorWithSkin(NABabyColor color, NAUIImageSkin skin){
  uint8 skinColor[4];
  switch(skin){
  case NA_UIIMAGE_SKIN_LIGHT:
    skinColor[0] = 255;
    skinColor[1] = 128;
    skinColor[2] = 16;
    skinColor[3] = 255;
    break;
  case NA_UIIMAGE_SKIN_DARK:
    skinColor[0] = 255;
    skinColor[1] = 128;
    skinColor[2] = 16;
    skinColor[3] = 255;
    break;
  default:
    #if NA_DEBUG
    naError("Cannot provide color for plain skin");
    #endif
    break;
  }
  naFillBabyColorWithu8(color, skinColor, NA_COLOR_BUFFER_RGBA);
}



NA_DEF NABabyImage* naCreateBabyImageFromNativeImage(const void* nativeImage){
  BYTE* lpPixels;
  NABabyImage* babyImage;

  HDC hdcSource = GetDC(NA_NULL); // the source device context
  HBITMAP hSource = (HBITMAP)nativeImage; // the bitmap selected into the device context

  BITMAPINFO MyBMInfo = {0};
  MyBMInfo.bmiHeader.biSize = sizeof(MyBMInfo.bmiHeader);

  // Get the BITMAPINFO structure from the bitmap
  GetDIBits(hdcSource, hSource, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS);

  // create the pixel buffer
  lpPixels = naMalloc(MyBMInfo.bmiHeader.biSizeImage);

  MyBMInfo.bmiHeader.biBitCount = 32;
  MyBMInfo.bmiHeader.biCompression = BI_RGB;  // no compression -> easier to use

  // Call GetDIBits a second time, this time to (format and) store the actual
  // bitmap data (the "pixels") in the buffer lpPixels
  GetDIBits(hdcSource, hSource, 0, MyBMInfo.bmiHeader.biHeight, lpPixels, &MyBMInfo, DIB_RGB_COLORS);

  babyImage = naCreateBabyImage(naMakeSizei(MyBMInfo.bmiHeader.biWidth, MyBMInfo.bmiHeader.biHeight), NA_NULL);
  // Windows does store an alpha component but it is not in use and therefore zero.
  // We therefore ignore it in the following call and receyve a completely opaque image.
  naFillBabyImageWithu8(babyImage, lpPixels, NA_FALSE, NA_COLOR_BUFFER_BGR0);
  naFree(lpPixels);

  // clean up: deselect bitmap from device context, close handles, delete buffer
  return babyImage;
}



NA_DEF NABabyImage* naCreateBabyImageFromFilePath(const NAUTF8Char* pathStr){
  // Currently, only png is possible
  NAPNG* png = naNewPNGWithPath(pathStr);
  NABabyImage* babyImage = naCreateBabyImageFromPNG(png);
  naDelete(png);
  return babyImage;
}



NA_DEF void* naAllocNativeImageWithBabyImage(const NABabyImage* image){
  HBITMAP hNewBitmap;

  NASizei size = naGetBabyImageSize(image);
  NAByte* buffer = naMalloc(size.width * size.height * 4);
  float* babyptr = naGetBabyImageData(image);

  naConvertBabyImageTou8(image, buffer, NA_TRUE, NA_COLOR_BUFFER_BGRA);

  hNewBitmap = CreateBitmap((int)size.width, (int)size.height, 1, 32, buffer);
  naFree(buffer);

  return hNewBitmap;
}



NA_DEF void naDeallocNativeImage(void* nativeImage){
  DeleteObject(nativeImage);
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
