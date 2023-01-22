
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NACocoaNativeSpace

- (id _Nonnull) initWithSpace:(NACocoaSpace* _Nonnull)newCocoaSpace frame:(NSRect)frame{
  self = [super initWithFrame:frame];

  // todo: make this dependent on whether tracking is needed or not.
  trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
      options:(NSTrackingAreaOptions)(NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow)
      owner:self userInfo:nil];
  [self addTrackingArea:trackingArea];
  [self setWantsLayer:YES];
  [self resetDrag];

  cocoaSpace = newCocoaSpace;
  return self;
}

- (void)dealloc{
  NA_COCOA_RELEASE(trackingArea);
  NA_COCOA_SUPER_DEALLOC();
}

- (void)drawRect:(NSRect)dirtyRect{
  [super drawRect:dirtyRect];
  if(cocoaSpace->space.backgroundColor[3] != 0.){
    [[NSColor colorWithDeviceRed:naUnlinearizeColorValue(cocoaSpace->space.backgroundColor[0])
      green:naUnlinearizeColorValue(cocoaSpace->space.backgroundColor[1])
      blue:naUnlinearizeColorValue(cocoaSpace->space.backgroundColor[2])
      alpha:cocoaSpace->space.backgroundColor[3]] setFill];
    NSRectFill(dirtyRect);
  }
  if(cocoaSpace->space.alternateBackground){
    [[[NSColor controlTextColor] colorWithAlphaComponent:(CGFloat).075] setFill];
    NSRectFill(dirtyRect);
  }
}

- (void)mouseDown:(NSEvent* _Nonnull)event{
  if(cocoaSpace->space.dragsWindow){
    isMoving = NA_TRUE;
    originMousePos = naMakePosWithNSPoint([event locationInWindow]);
  }else{
    [super mouseDown:event];
    na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_DOWN);
  }
}

- (void)mouseDragged:(NSEvent* _Nonnull)event{
  if(cocoaSpace->space.dragsWindow && isMoving){
    NAPos curMousePos = naMakePosWithNSPoint([event locationInWindow]);
    NSRect frame = [[self window] frame];
    frame.origin.x += curMousePos.x - originMousePos.x;
    frame.origin.y += curMousePos.y - originMousePos.y;
    [[self window] setFrame:frame display:YES];
  }else{
    [super mouseDragged:event];
    na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_MOVED);
  }
}

- (void)mouseUp:(NSEvent* _Nonnull)event{
  if(cocoaSpace->space.dragsWindow){
    [self resetDrag];
  }else{
    [super mouseUp:event];
    na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_UP);
  }
}

- (void)mouseMoved:(NSEvent* _Nonnull)event{
  NA_UNUSED(event);
  na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_MOVED);
}

- (void)mouseEntered:(NSEvent* _Nonnull)event{
  NA_UNUSED(event);
  na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_ENTERED);
}

- (void)mouseExited:(NSEvent* _Nonnull)event{
  NA_UNUSED(event);
  na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_EXITED);
}

- (void)resetDrag{
  isMoving = NA_FALSE;
  originMousePos = naMakePos(0, 0);
}

- (BOOL)acceptsFirstMouse:(NSEvent* _Nullable)event{
  NA_UNUSED(event);
  return YES;
}

- (void)cancelOperation:(nullable id)sender{
  [self resetDrag];
}

@end



NA_DEF NASpace* _Nonnull naNewSpace(NASize size){
  NACocoaSpace* cocoaSpace = naNew(NACocoaSpace);

  NACocoaNativeSpace* nativePtr = [[NACocoaNativeSpace alloc]
    initWithSpace:cocoaSpace
    frame:naMakeNSRectWithSize(size)];  
  na_InitSpace((NASpace*)cocoaSpace, NA_COCOA_PTR_OBJC_TO_C(nativePtr));

  cocoaSpace->space.backgroundColor[0] = 0.;
  cocoaSpace->space.backgroundColor[1] = 0.;
  cocoaSpace->space.backgroundColor[2] = 0.;
  cocoaSpace->space.backgroundColor[3] = 0.;
  cocoaSpace->space.alternateBackground = NA_FALSE;
  cocoaSpace->space.dragsWindow = NA_FALSE;

  return (NASpace*)cocoaSpace;
}



NA_DEF void na_DestructCocoaSpace(NACocoaSpace* _Nonnull cocoaSpace){
  na_ClearSpace((NASpace*)cocoaSpace);
}



NA_DEF void naSetUIElementRect(NASpace* _Nonnull space, NARect rect){
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  NSRect frame = naMakeNSRectWithRect(rect);
  [nativePtr setFrame: frame];
}



NA_DEF void naSetSpaceVisible(NASpace* _Nonnull space, NABool visible){
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  [nativePtr setHidden:visible ? NO : YES];
}



NA_DEF void naSetSpaceDragsWindow(NASpace* _Nonnull space, NABool isDraggable){
  space->dragsWindow = isDraggable;
}



@class NACocoaNativeRadio;
@class NACocoaNativeTextBox;


NA_HDEF NSView* _Nonnull na_getNSViewOfChild(void* _Nonnull child){
  naDefineCocoaObjectConst(NSView<NACocoaNativeEncapsulatedElement>, cocoaView, child);

  NSView* childView;  
  switch(naGetUIElementType(child)){
  case NA_UI_RADIO:
    childView = [cocoaView getEncapsulatingView];
    break;
  case NA_UI_TEXTBOX:
    childView = [cocoaView getEncapsulatingView];
    break;
  default:
    childView = cocoaView;
    break;
  }
  
  return childView;
}



NA_DEF void naAddSpaceChild(NASpace* _Nonnull space, void* _Nonnull child, NAPos pos){
  naDefineCocoaObject(NACocoaNativeSpace, nativeSpacePtr, space);

  NSView* childView = na_getNSViewOfChild(child);  
  
  double offsetY = na_GetUIElementYOffset(child);
  
  [nativeSpacePtr addSubview:childView];
  NSRect frame = [childView frame];
  frame.origin = NSMakePoint((CGFloat)pos.x, (CGFloat)pos.y + offsetY);
  [childView setFrame: frame];
  
  na_AddSpaceChild(space, child);
}



NA_DEF void naRemoveSpaceChilds(NASpace* _Nonnull space)
{
  while(!naIsListEmpty(&(space->childs))){
    void* child = naGetListFirstMutable(&(space->childs));
    na_RemoveSpaceChild(space, child);
    [(NA_COCOA_BRIDGE NSView*)(naGetUIElementNativePtr(child)) removeFromSuperview];
  }
}



NA_DEF void naShiftSpaceChilds(NASpace* _Nonnull space, NAPos shift)
{
  NAListIterator childIt = naMakeListMutator(&(space->childs));
  while(naIterateList(&childIt)){
    void* child = naGetListCurMutable(&childIt);
    NSView* childView = na_getNSViewOfChild(child);  
    NSRect frame = [childView frame];
    frame.origin = NSMakePoint(frame.origin.x + shift.x, frame.origin.y + shift.y);
    [childView setFrame: frame];
  }
  naClearListIterator(&childIt);
}



NA_HDEF NARect na_GetSpaceAbsoluteInnerRect(const NA_UIElement* _Nonnull space){
  naDefineCocoaObjectConst(NACocoaNativeSpace, nativePtr, space);
  // Warning: does not work when frame unequal bounds.
  NSRect contentRect = [nativePtr frame];

  const void* parent = naGetUIElementParentConst(space);
  if(parent){
    NARect parentRect = naGetUIElementRectAbsolute(naGetUIElementParentConst(space));
    NARect relRect = naMakeRectWithNSRect([nativePtr frame]);
    return naMakeRect(
      naMakePos(parentRect.pos.x + relRect.pos.x, parentRect.pos.y + relRect.pos.y),
      relRect.size);
  }else{
    NARect windowRect;
    if([nativePtr window]){
      #if NA_DEBUG
        //naError("Given element has no NAWindow as parent. Using native window parent.");
      #endif
      windowRect = naMakeRectWithNSRect([[nativePtr window] frame]);
    }else{
      // The space has not (yet) been added to a window. Just use zero values.
      // Do not emit a warning as this is quite a common case when trying to
      // add a space to a super-space using the information of its size.
      windowRect = naMakeRectZero();
    }
    return naMakeRectS(
      windowRect.pos.x + contentRect.origin.x,
      windowRect.pos.y + contentRect.origin.y,
      contentRect.size.width,
      contentRect.size.height);
  }
}



NA_DEF void naSetSpaceBackgroundColor(NASpace* _Nonnull space, const NABabyColor* _Nullable color){
  if(color){
    space->backgroundColor[0] = (*color)[0];
    space->backgroundColor[1] = (*color)[1];
    space->backgroundColor[2] = (*color)[2];
    space->backgroundColor[3] = (*color)[3];
  }else{
    space->backgroundColor[3] = 0.;
  }
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  [nativePtr setNeedsDisplay:YES];
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
