
#if defined NA_BUTTON_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_BUTTON_INCLUDED
#define NA_BUTTON_INCLUDED



#define NA_BUTTON_BORDERED   0x01
#define NA_BUTTON_PUSH       0x00
#define NA_BUTTON_STATEFUL   0x02

// Creates either a text or image button.
// The flags is a combination of the macros defined above.
// Default height for TextPushButton is 24.
// Default height for TextStateButton is 25.
NA_API NAButton* naNewTextPushButton(
  const NAUTF8Char* text,
  double width);
NA_API NAButton* naNewTextStateButton(
  const NAUTF8Char* textOff,
  const NAUTF8Char* textOn,
  double width);
NA_API NAButton* naNewImagePushButton(
  const NAUIImage* uiImage,
  NASize size,
  NABool bordered);
NA_API NAButton* naNewImageStateButton(
  const NAUIImage* uiImageOff,
  const NAUIImage* uiImageOn,
  NASize size,
  NABool bordered);

// Changes the visibility or enabled state of the button.
NA_API void naSetButtonVisible(NAButton* button, NABool visible);
NA_API void naSetButtonEnabled(NAButton* button, NABool enabled);

// Gets or sets the state of the button.
// Will emit an error if this is a stateless button.
NA_API NABool naGetButtonState(const NAButton* button);
NA_API void naSetButtonState(NAButton* button, NABool state);

// Sets the text or the image for the button. Use Off function for push buttons.
// Will emit an error if the button was not created with the matching type.
NA_API void naSetButtonTextOff(NAButton* button, const NAUTF8Char* text);
NA_API void naSetButtonTextOn(NAButton* button, const NAUTF8Char* text);
NA_API void naSetButtonImageOff(NAButton* button, const NAUIImage* uiImage);
NA_API void naSetButtonImageOn(NAButton* button, const NAUIImage* uiImage);

// Returns the display flags of a button.
NA_API NABool naIsButtonStateful(NAButton* button);
NA_API NABool naIsButtonBordered(NAButton* button);
NA_API NABool naIsButtonTextual(NAButton* button);

// Defines this button to be a submit or abort button.
// A submit button will be shown visually prominent and serves as the default
// button of a window, often times labeled 'Ok'. The abort button is a button
// which is pressed when the window shall be dismissed (often times labeled as
// 'Cancel'). The following keyboard shortcut reactions are mapped to the
// given handler with the given controller argument:
// - default/submit: Return + Enter
// - abort/cancel:   Escape + Cmd-Dot (on macOS)
NA_API void naSetButtonSubmit(
  NAButton* button,
  NAReactionHandler handler,
  void* controller);
NA_API void naSetButtonAbort(
  NAButton* button,
  NAReactionHandler handler,
  void* controller);



#endif // NA_BUTTON_INCLUDED



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
