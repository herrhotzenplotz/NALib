
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NAWINAPIColor* naGetWINAPISpaceBackgroundColor(NAWINAPISpace* winapiSpace);



NAWINAPICallbackInfo naSpaceWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  RECT spaceRect;
  NA_UIElement* childelement;
  NAWINAPISpace* winapiSpace = (NAWINAPISpace*)uiElement;
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  NAWINAPIColor* bgColor;

  switch(message){
  case WM_SHOWWINDOW:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_WINDOWPOSCHANGED:
  case WM_MOVE:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_PRINTCLIENT:
  case WM_CTLCOLORBTN:
  case WM_CTLCOLOREDIT:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_STYLECHANGING:
  case WM_STYLECHANGED:
  case WM_SETTEXT:
  case WM_GETFONT:
  case WM_SETFONT:
  case WM_GETTEXTLENGTH:
  case WM_GETTEXT:
  case WM_MOUSEMOVE: // captured in naUIElementWINAPIProc
  case WM_MOUSELEAVE: // captured in naUIElementWINAPIProc
  case WM_PARENTNOTIFY:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_IME_SETCONTEXT:
  case WM_IME_NOTIFY:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_LBUTTONUP:
  case WM_KEYDOWN:
  case WM_CHAR:
  case WM_KEYUP:
  case 0x43c: // undocumented
    break;

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    // Do not change the default behaviour of focus. Otherwise, this would cause
    // labels and textFields to not display a cursor or selection.
    break;

  // Dialog messages
  case WM_UPDATEUISTATE:
  case WM_GETDLGCODE:
  case WM_CHANGEUISTATE:

  // Cases being called due to bubbling the message.
  case BM_SETCHECK:
  case EM_GETSEL:
  case EM_GETRECT:
  case EM_CHARFROMPOS:
  case EM_LINEFROMCHAR:
  case EM_POSFROMCHAR:
  case EM_SETSEL:
    break;

  case WM_CTLCOLORSTATIC:
    // Message is sent to parent space.
    // wParam: device context
    // lParam HWND handle to actual control
    // return: background color brush
    childelement = (NA_UIElement*)na_GetUINALibEquivalent((HWND)lParam);
    switch(childelement->elementType){
    case NA_UI_LABEL:
      if(naIsLabelEnabled((NALabel*)childelement)){
        SetTextColor((HDC)wParam, app->fgColor.color);
      }else{
        SetTextColor((HDC)wParam, app->fgColorDisabled.color);
      }
      bgColor = naGetWINAPISpaceBackgroundColor(uiElement);
      SetBkColor((HDC)wParam, bgColor->color);
      info.result = (LRESULT)bgColor->brush;
      info.hasBeenHandeled = NA_TRUE;
      break;
    }
    break;

  case WM_ERASEBKGND: // wParam: Device context, return > 1 if erasing, 0 otherwise
    GetClientRect(naGetUIElementNativePtr(uiElement), &spaceRect);
    bgColor = naGetWINAPISpaceBackgroundColor(uiElement);
    if(bgColor != winapiSpace->lastBgColor){ // Only draw if changed
      FillRect((HDC)wParam, &spaceRect, bgColor->brush);
      winapiSpace->lastBgColor = bgColor;
    }
    info.hasBeenHandeled = NA_TRUE;
    info.result = 1;
    break;

  default:
    //printf("Uncaught Space message" NA_NL);
    break;
  }
  
  return info;
}



NAWINAPIColor* naGetWINAPISpaceBackgroundColor(NAWINAPISpace* winapiSpace){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  NAWINAPIColor* retcolor;
  NAInt alternateLevel = 0;
  void* parent = winapiSpace;
  while(parent){
    if(naGetSpaceAlternateBackground(parent)){alternateLevel++;}
    parent = naGetUIElementParentSpace(parent);
  }
  switch(alternateLevel){
  case 0: retcolor = &(app->bgColor); break;
  case 1: retcolor = &(app->bgColorAlternate); break;
  case 2:
  default:
    retcolor = &(app->bgColorAlternate2); break;
  }
  return retcolor;
}



NA_DEF NASpace* naNewSpace(NASize size){
  NAWINAPISpace* winapiSpace = naNew(NAWINAPISpace);

  winapiSpace->rect = naMakeRect(naMakePos(0., 0.), size);
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);

	HWND nativePtr = CreateWindow(
		TEXT("NASpace"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE,
    0,
    0,
    (int)(winapiSpace->rect.size.width * uiScale),
    (int)(winapiSpace->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  na_InitSpace(&(winapiSpace->space), nativePtr);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  winapiSpace->lastBgColor = &(app->bgColor);

  winapiSpace->space.backgroundColor[0] = 0.;
  winapiSpace->space.backgroundColor[1] = 0.;
  winapiSpace->space.backgroundColor[2] = 0.;
  winapiSpace->space.backgroundColor[3] = 0.;
  winapiSpace->space.alternateBackground = NA_FALSE;

  return (NASpace*)winapiSpace;
}



NA_DEF void na_DestructWINAPISpace(NAWINAPISpace* winapiSpace){
  na_ClearSpace((NASpace*)winapiSpace);
}



NA_DEF void naAddSpaceChild(NASpace* space, void* child, NAPos pos){
  na_AddSpaceChild(space, child);

  double offsetY = na_GetUIElementYOffset(child);

  NARect childRect = naGetUIElementRect(child);
  childRect.pos = naMakePos(pos.x, pos.y + offsetY);
  naSetUIElementRect(child, childRect);
}



NA_DEF void naSetSpaceBackgroundColor(NASpace* space, const NABabyColor color){
  // todo
}



NA_DEF void naRemoveSpaceChilds(NASpace* space){
  // todo
}



NA_DEF void naShiftSpaceChilds(NASpace* space, NAPos shift){
  NAWINAPISpace* winapiSpace = (NAWINAPISpace*)space;

  NAListIterator childIt = naMakeListMutator(&(space->childs));
  while(naIterateList(&childIt)){
    void* child = naGetListCurMutable(&childIt);
    NARect elementRect = naGetUIElementRect(child);
    elementRect.pos.x += shift.x;
    elementRect.pos.y += shift.y;
    naSetUIElementRect(child, elementRect);
  }
  naClearListIterator(&childIt);
}



NA_DEF void naSetSpaceVisible(NASpace* space, NABool visible){
  // todo
}



NA_HDEF void naSetSpaceDragsWindow(NASpace* space, NABool isDraggable){
  // todo
}



NA_HDEF NARect na_GetSpaceRect(const NA_UIElement* space){
  NAWINAPISpace* winapiSpace = (NAWINAPISpace*)space;
  return winapiSpace->rect;
}



NA_HDEF void na_SetSpaceRect(NA_UIElement* space, NARect rect){
  NAWINAPISpace* winapiSpace = (NAWINAPISpace*)space;

  winapiSpace->rect = rect;
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(space));

  SetWindowPos(
    naGetUIElementNativePtr(space),
    HWND_TOP,
    (int)(winapiSpace->rect.pos.x * uiScale),
    (int)((parentRect.size.height - winapiSpace->rect.pos.y - winapiSpace->rect.size.height) * uiScale),
    (int)(winapiSpace->rect.size.width * uiScale),
    (int)(winapiSpace->rect.size.height * uiScale),
    SWP_NOZORDER);
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
