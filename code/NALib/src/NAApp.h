
#ifndef NA_APP_INCLUDED
#define NA_APP_INCLUDED

#ifdef __cplusplus
  extern "C"{
#endif

#include "NABase.h"
#if NA_COMPILE_GUI == 1

// NALib provides a simple implementation for Graphical User Interfaces (GUI).
//
// GUIs are implemented in various ways across all systems and there is no
// general solution to how one should design a GUI. Therefore, NALib combines
// several ideas into a new implementation but allowing the programmer to use
// that implementation consistently on all systems. Currently Mac and Windows.
//
// In NALib, you create a GUI by starting an NAApplication and then adding
// user interface elements to that application one by one. The topmost ui
// element is the application itself. From there, you can access the screens
// and add Windows. Windows may contain spaces and subspaces where you place
// elements like buttons, sliders, OpenGL spaces, etc.
//
// User interactions can be captured by defining so called "reactions".
//
// Note that UI implementation of NALib automatically makes use of
// multi-threading. Therefore, when including this file on Mac OS X,
// grand central dispatch (GCD) will automatically be included.
// On Windows, the default WINAPI threads will be used.



#include "NAUtility/NAString.h"

#include "NAApp/NAFont.h"
#include "NAApp/NAKeyboard.h"
#include "NAApp/NAPreferences.h"
#include "NAApp/NAReaction.h"
#include "NAApp/NAUIImage.h"
#include "NAApp/NAMouse.h"

// The base of every ui element.
#include "NAApp/NAUIElement.h"

// The acutal ui elements
#include "NAApp/NAApplication.h"
#include "NAApp/NAButton.h"
#include "NAApp/NACheckBox.h"
#include "NAApp/NAImageSpace.h"
#include "NAApp/NALabel.h"
#include "NAApp/NAMenu.h"
#include "NAApp/NAMenuItem.h"
#include "NAApp/NAMetalSpace.h"
#include "NAApp/NAOpenGLSpace.h"
#include "NAApp/NARadio.h"
#include "NAApp/NAScreen.h"
#include "NAApp/NASelect.h"
#include "NAApp/NASlider.h"
#include "NAApp/NASpace.h"
#include "NAApp/NATextBox.h"
#include "NAApp/NATextField.h"
#include "NAApp/NAWindow.h"



#endif // NA_COMPILE_GUI == 1

#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_APP_INCLUDED



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
