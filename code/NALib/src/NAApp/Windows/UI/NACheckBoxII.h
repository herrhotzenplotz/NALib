
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NAWINAPICallbackInfo naCheckBoxWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_SETFONT:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_WINDOWPOSCHANGED:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case BM_SETCHECK:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_ERASEBKGND:
  case WM_GETTEXTLENGTH:
  case WM_GETTEXT:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_IME_SETCONTEXT:
  case BM_SETSTATE:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_IME_NOTIFY:
  case BM_GETSTATE:
  case WM_LBUTTONUP:
    break;

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    // We do not display any caret.
    info.hasBeenHandeled = NA_TRUE;
    info.result = 0;
    break;

  default:
    //printf("Uncaught CheckBox message" NA_NL);
    break;
  }
  
  return info;
}



NABool naCheckBoxWINAPINotify(void* uiElement, WORD notificationCode){
  NABool hasBeenHandeled = NA_FALSE;
  NABool check;

  switch(notificationCode){
    case BN_CLICKED:
      ReleaseCapture();
      check = naGetCheckBoxState(uiElement);
      naSetCheckBoxState(uiElement, !check);
      na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_PRESSED);
      hasBeenHandeled = NA_TRUE;
      break;
  }

  return hasBeenHandeled;
}



NA_DEF NACheckBox* naNewCheckBox(const NAUTF8Char* text, double width){
  NAWINAPICheckBox* winapiCheckBox = naNew(NAWINAPICheckBox);

  TCHAR* systemText = naAllocSystemStringWithUTF8String(text);

  double uiScale = naGetUIElementResolutionFactor(NA_NULL);
  winapiCheckBox->rect = naMakeRectS(0., 0., width, 18.);

	HWND nativePtr = CreateWindow(
		TEXT("BUTTON"),
    systemText,
    WS_CHILD | WS_VISIBLE | BS_LEFT | BS_VCENTER | BS_TEXT | BS_CHECKBOX,
    0,
    0,
    (int)(winapiCheckBox->rect.size.width * uiScale),
    (int)(winapiCheckBox->rect.size.height * uiScale),
		naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  naFree(systemText);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldCheckBoxWindowProc){app->oldCheckBoxWindowProc = oldproc;}

  na_InitCheckBox(&(winapiCheckBox->checkBox), nativePtr);

  SendMessage(nativePtr, WM_SETFONT, (WPARAM)naGetFontNativePointer(naGetSystemFont()), MAKELPARAM(TRUE, 0));

  return (NACheckBox*)winapiCheckBox;
}



NA_DEF void na_DestructWINAPICheckBox(NAWINAPICheckBox* winapiCheckBox){
  na_ClearCheckBox((NACheckBox*)winapiCheckBox);
}



NA_DEF void naSetCheckBoxVisible(NACheckBox* checkBox, NABool visible){
  //todo
}



NA_DEF void naSetCheckBoxEnabled(NACheckBox* checkBox, NABool enabled){
  EnableWindow(naGetUIElementNativePtr(checkBox), enabled);
}



NA_DEF NABool naGetCheckBoxState(const NACheckBox* checkBox){
  LPARAM state = SendMessage(naGetUIElementNativePtrConst(checkBox), BM_GETSTATE, 0, 0);
  return (state & BST_CHECKED) == BST_CHECKED;
}



NA_DEF void naSetCheckBoxState(NACheckBox* checkBox, NABool state){
  LPARAM lParam = state ? BST_CHECKED : BST_UNCHECKED;
  SendMessage(naGetUIElementNativePtr(checkBox), BM_SETCHECK, lParam, 0);
}



NA_DEF void naSetCheckBoxTextColor(NACheckBox* checkBox, const NABabyColor* color){
  // todo
}



NA_HDEF NARect na_GetCheckBoxRect(const NA_UIElement* checkBox)
{
  const NAWINAPICheckBox* winapiCheckBox = (const NAWINAPICheckBox*)checkBox;
  return winapiCheckBox->rect;
}

NA_HDEF void na_SetCheckBoxRect(NA_UIElement* checkBox, NARect rect){
  NAWINAPICheckBox* winapiCheckBox = (NAWINAPICheckBox*)checkBox;

  winapiCheckBox->rect = rect;
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(checkBox));

  SetWindowPos(
    naGetUIElementNativePtr(checkBox),
    HWND_TOP,
    (int)(winapiCheckBox->rect.pos.x * uiScale),
    (int)((parentRect.size.height - winapiCheckBox->rect.pos.y - winapiCheckBox->rect.size.height) * uiScale),
    (int)(winapiCheckBox->rect.size.width * uiScale),
    (int)(winapiCheckBox->rect.size.height * uiScale),
    0);
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
