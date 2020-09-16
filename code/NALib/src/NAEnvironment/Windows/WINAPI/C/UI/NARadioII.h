
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPIRadio NAWINAPIRadio;
struct NAWINAPIRadio {
  NARadio radio;
};



NAWINAPICallbackInfo naRadioWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  NABool check;

  switch(message){
  case WM_SETFONT:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_WINDOWPOSCHANGED:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_ERASEBKGND:
  case WM_GETTEXTLENGTH:
  case WM_GETTEXT:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_MOUSEACTIVATE:
  case WM_IME_SETCONTEXT:
  case WM_SETFOCUS:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_KILLFOCUS:
  case WM_IME_NOTIFY:
  case WM_GETDLGCODE:
  case WM_STYLECHANGING:
  case WM_STYLECHANGED:
  case WM_LBUTTONDOWN:
  case BM_SETSTATE: // only highlighting, not state changing.
  case BM_SETCHECK: // when pressed the button or manually sends BM_SETCHECK
    break;

  case WM_LBUTTONUP:
    ReleaseCapture();
    check = naGetRadioState(uiElement);
    naSetRadioState(uiElement, !check);
    na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_PRESSED);
    info.hasbeenhandeled = NA_TRUE;
    info.result = 0;
    break;

  default:
    //printf("Uncaught Radio message" NA_NL);
    break;
  }
  
  return info;
}



NA_DEF NARadio* naNewRadio(const NAUTF8Char* text, NASize size){
  HWND hWnd;
  DWORD style;
  TCHAR* systemtext;
  WNDPROC oldproc;

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();

  NAWINAPIRadio* winapiRadio = naAlloc(NAWINAPIRadio);

  style = WS_CHILD | WS_VISIBLE | BS_LEFT | BS_VCENTER | BS_TEXT | BS_RADIOBUTTON;

  systemtext = naAllocSystemStringWithUTF8String(text);

	hWnd = CreateWindow(
		TEXT("BUTTON"), systemtext, style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeId(naGetApplication()), NULL );
  
  naFree(systemtext);

  oldproc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldRadioWindowProc){app->oldRadioWindowProc = oldproc;}

  na_InitRadio(&(winapiRadio->radio), hWnd);

  SendMessage(hWnd, WM_SETFONT, (WPARAM)na_GetFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NARadio*)winapiRadio;
}



NA_DEF void na_DestructRadio(NARadio* radio){
  NAWINAPIRadio* winapiRadio = (NAWINAPIRadio*)radio;
  na_ClearRadio(&(winapiRadio->radio));
}



NA_HDEF NARect na_GetRadioAbsoluteInnerRect(NA_UIElement* radio){
  NA_UNUSED(radio);
  return naMakeRectS(20, 40, 100, 50);
}



NA_DEF NABool naGetRadioState(NARadio* radio){
  LPARAM state = SendMessage(naGetUIElementNativeId(radio), BM_GETSTATE, 0, 0);
  return (state & BST_CHECKED) == BST_CHECKED;
}



NA_DEF void naSetRadioState(NARadio* radio, NABool state){
  LPARAM lParam = state ? BST_CHECKED : BST_UNCHECKED;
  SendMessage(naGetUIElementNativeId(radio), BM_SETCHECK, lParam, 0);
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
