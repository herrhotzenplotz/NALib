
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "NAPreferences.h"



@implementation NACocoaNativeWindow

- (id) initWithWindow:(NACocoaWindow*)newCocoaWindow contentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen{
  self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
  cocoaWindow = newCocoaWindow;
  trackingCount = 0;
  trackingArea = nil;
  [self setReleasedWhenClosed:NO];
  [self setHasShadow:YES];
  return self;
}

- (void)dealloc{
  NA_COCOA_RELEASE(trackingArea);
  NA_COCOA_SUPER_DEALLOC();
}

- (NACocoaWindow*) window{
  return cocoaWindow;
}

- (NSTrackingArea*) trackingArea{
  return trackingArea;
}

- (size_t) trackingCount{
  return trackingCount;
}

- (BOOL)windowShouldClose:(id)sender{
  NABool shouldClose;
  NA_UNUSED(sender);
  naSetFlagu32(&(cocoaWindow->window.coreFlags), NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE, NA_TRUE);
  na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_CLOSES);
  shouldClose = !naGetFlagu32(cocoaWindow->window.coreFlags, NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING);
  naSetFlagu32(&(cocoaWindow->window.coreFlags), NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE | NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, NA_FALSE);
  return (BOOL)shouldClose;
}

- (NARect)getContentRect{
  NSRect contentRect = [NSWindow contentRectForFrameRect:[super frame] styleMask:[self styleMask]];
  return naMakeRectWithNSRect(contentRect);
}

- (void)setContentRect:(NARect)rect{
  NSRect frame = [NSWindow frameRectForContentRect:naMakeNSRectWithRect(rect) styleMask:[self styleMask]];
  [super setFrame:frame display:YES];
}

- (void)setWindowTitle:(const NAUTF8Char*) title{
  [self setTitle:[NSString stringWithUTF8String:title]];
}

- (void)setKeepOnTop:(NABool) keepOnTop{
  if(keepOnTop){
    [self setLevel:NSFloatingWindowLevel];
  }else{
    [self setLevel:NSNormalWindowLevel];
  }
}

- (void)renewMouseTracking{
  NA_COCOA_RELEASE(trackingArea);
  trackingArea = [[NSTrackingArea alloc] initWithRect:[[self contentView] bounds]
      options:(NSTrackingAreaOptions)(NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveWhenFirstResponder)
      owner:self userInfo:nil];
  [[self contentView] addTrackingArea:trackingArea];
}

- (void)clearMouseTracking{
  [[self contentView] removeTrackingArea:trackingArea];
  NA_COCOA_RELEASE(trackingArea);
  trackingArea = nil;
}

- (void)retainMouseTracking{
  trackingCount++;
  if(trackingCount == 1){
    [self setAcceptsMouseMovedEvents:YES];
    na_RenewWindowMouseTracking((NAWindow*)cocoaWindow);
  }
}

- (void)releaseMouseTracking{
  trackingCount--;
  if(trackingCount == 0){
    [self setAcceptsMouseMovedEvents:NO];
    na_ClearWindowMouseTracking((NAWindow*)cocoaWindow);
  }
}

- (void)mouseMoved:(NSEvent*)event{
  na_SetMouseMovedTo(naMakePosWithNSPoint([NSEvent mouseLocation]));
  na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_MOUSE_MOVED);
//  [NSEvent setMouseCoalescingEnabled:NO];
}

- (void)mouseEntered:(NSEvent*)event{
  NA_UNUSED(event);
  na_SetMouseEnteredAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
  na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_MOUSE_ENTERED);
}

- (void)mouseExited:(NSEvent*)event{
  NA_UNUSED(event);
  na_SetMouseExitedAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
  na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_MOUSE_EXITED);
}

- (void)keyUp:(NSEvent*)event{
  NA_UNUSED(event);
  na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_KEY_UP);
}

- (void)windowDidResize:(NSNotification *)notification{
  NA_UNUSED(notification);
  na_RememberWindowPosition((NAWindow*)cocoaWindow);
  na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_RESHAPE);
}

- (void)windowDidMove:(NSNotification *)notification{
  NA_UNUSED(notification);
  na_RememberWindowPosition((NAWindow*)cocoaWindow);
  na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_RESHAPE);
}

- (BOOL)canBecomeMainWindow{
  return !naGetFlagu32(cocoaWindow->window.flags, NA_WINDOW_AUXILIARY);
}

- (BOOL)canBecomeKeyWindow{
  return (BOOL)naGetFlagu32(cocoaWindow->window.coreFlags, NA_CORE_WINDOW_FLAG_ACCEPTS_KEY_REACTIONS);
}

@end



NA_DEF NAWindow* naNewWindow(const NAUTF8Char* title, NARect rect, uint32 flags, NAInt storageTag){
  NACocoaWindow* cocoaWindow = naNew(NACocoaWindow);

  NABool resizeable = naGetFlagu32(flags, NA_WINDOW_RESIZEABLE);
  NABool titleless = naGetFlagu32(flags, NA_WINDOW_TITLELESS);
  NABool noncloseable = naGetFlagu32(flags, NA_WINDOW_NON_CLOSEABLE);
  NABool nonminiaturizeable = naGetFlagu32(flags, NA_WINDOW_NON_MINIATURIZEABLE);
  NABool auxiliary = naGetFlagu32(flags, NA_WINDOW_AUXILIARY);
  
  rect = naSetWindowStorageTag((NAWindow*)cocoaWindow, storageTag, rect, resizeable);

  NSUInteger styleMask = 0;
  if(resizeable){styleMask |= NAWindowStyleMaskResizable;}
  if(!titleless){styleMask |= NAWindowStyleMaskTitled;}
  if(!noncloseable){styleMask |= NAWindowStyleMaskClosable;}
  if(!nonminiaturizeable){styleMask |= NAWindowStyleMaskMiniaturizable;}
//  if(auxiliary){styleMask |= NAWindowStyleMaskNonactivatingPanel | NAWindowStyleMaskUtilityWindow;}
  
  NACocoaNativeWindow* nativePtr = [[NACocoaNativeWindow alloc]
    initWithWindow:cocoaWindow
    contentRect:naMakeNSRectWithRect(rect)
    styleMask:styleMask
    backing:NSBackingStoreBuffered
    defer:NO
    screen:nil];
  
  if(auxiliary){
    [nativePtr setKeepOnTop:YES];
    [nativePtr setHidesOnDeactivate:YES];
    [nativePtr setCollectionBehavior:NSWindowCollectionBehaviorTransient | NAWindowCollectionBehaviorFullScreenAuxiliary];
    [nativePtr setExcludedFromWindowsMenu:YES];
  }
  
  [nativePtr setDelegate:nativePtr];
  [nativePtr setTitle:[NSString stringWithUTF8String:title]];
  [nativePtr setInitialFirstResponder:[nativePtr contentView]];
  na_InitWindow((NAWindow*)cocoaWindow, NA_COCOA_PTR_OBJC_TO_C(nativePtr), NA_NULL, NA_FALSE, resizeable, rect);

  cocoaWindow->window.flags = flags;

  NASpace* space = naNewSpace(rect.size);
  naSetWindowContentSpace((NAWindow*)cocoaWindow, space);

  na_SetUIElementParent((NA_UIElement*)cocoaWindow, naGetApplication(), NA_TRUE);

  return (NAWindow*)cocoaWindow;
}



NA_DEF void na_DestructCocoaWindow(NACocoaWindow* cocoaWindow){
  naDefineCocoaObject(NACocoaNativeWindow, nativePtr, cocoaWindow);
  [nativePtr close];
  
  na_ClearWindow((NAWindow*)cocoaWindow);
}



NA_DEF void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title){
  naDefineCocoaObject(NACocoaNativeWindow, nativePtr, window);
  [nativePtr setWindowTitle:title];
}



NA_DEF void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop){
  naDefineCocoaObject(NACocoaNativeWindow, nativePtr, window);
  [nativePtr setKeepOnTop:keepOnTop];
}



NA_DEF double naGetWindowUIResolution(const NAWindow* window){
  naDefineCocoaObjectConst(NACocoaNativeWindow, nativePtr, window);
  CGFloat uiScale = naGetWindowBackingScaleFactor(nativePtr);

  return (uiScale == 1.) ? NA_UIIMAGE_RESOLUTION_SCREEN_1x : NA_UIIMAGE_RESOLUTION_SCREEN_2x;
}



NA_DEF void naSetWindowFirstTabElement(NAWindow* window, const void* firstTabElem){
  naDefineCocoaObject(NACocoaNativeWindow, nativePtr, window);
  naDefineCocoaObjectConst(NSView, cocoaFirstTab, firstTabElem);
  [nativePtr setInitialFirstResponder:cocoaFirstTab];
}



NA_HDEF NARect na_GetNativeWindowAbsoluteInnerRect(const NSWindow* window){
  NARect rect;
  NSRect contentRect;
  NSRect windowFrame;
  contentRect = [[window contentView] frame];
  windowFrame = [window frame];
  rect.pos.x = windowFrame.origin.x + contentRect.origin.x;
  rect.pos.y = windowFrame.origin.y + contentRect.origin.y;
  rect.size.width = contentRect.size.width;
  rect.size.height = contentRect.size.height;
  return rect;
}



NA_HDEF NARect na_GetWindowAbsoluteInnerRect(const NA_UIElement* window){
  naDefineCocoaObjectConst(NACocoaNativeWindow, nativePtr, window);
  return na_GetNativeWindowAbsoluteInnerRect(nativePtr);
}



NA_HDEF void na_RenewWindowMouseTracking(NAWindow* window){
  naDefineCocoaObject(NACocoaNativeWindow, nativePtr, window);
  [nativePtr renewMouseTracking];
}



NA_HDEF void na_ClearWindowMouseTracking(NAWindow* window){
  naDefineCocoaObject(NACocoaNativeWindow, nativePtr, window);
  [nativePtr clearMouseTracking];
}



//NA_HDEF void* na_AllocMouseTracking(NAWindow* window){
//  naDefineCocoaObject(NACocoaNativeWindow, nativePtr, window);
//  [nativePtr retainMouseTracking];
//}
//
//
//
//NA_HDEF void na_DeallocMouseTracking(NAWindow* window){
//  naDefineCocoaObject(NACocoaNativeWindow, nativePtr, window);
//  [nativePtr releaseMouseTracking];
//}



NA_DEF NARect naGetMainScreenRect(){
  CGFloat uiScale = [[NSScreen mainScreen] backingScaleFactor];
  NARect rect = naMakeRectWithNSRect([[NSScreen mainScreen] frame]);
  rect.pos.x /= uiScale;
  rect.pos.y /= uiScale;
  rect.size.width /= uiScale;
  rect.size.height /= uiScale;
  return rect;
}



NA_DEF void naShowWindow(const NAWindow* window){
  naDefineCocoaObjectConst(NACocoaNativeWindow, nativePtr, window);
  [nativePtr makeKeyAndOrderFront:NA_NULL];
}



NA_DEF void naCloseWindow(const NAWindow* window){
  naDefineCocoaObjectConst(NACocoaNativeWindow, nativePtr, window);
  [nativePtr performClose:NA_NULL];
}



NA_DEF void naSetWindowContentSpace(NAWindow* window, void* space){
  #if NA_DEBUG
    if((naGetUIElementType(space) != NA_UI_SPACE) &&
      (naGetUIElementType(space) != NA_UI_IMAGE_SPACE) &&
      (naGetUIElementType(space) != NA_UI_OPENGL_SPACE) &&
      (naGetUIElementType(space) != NA_UI_METAL_SPACE))
      naError("Require a space, not any arbitrary ui element.");
  #endif

  naDefineCocoaObject(NACocoaNativeWindow, nativeWindowPtr, window);
  naDefineCocoaObjectConst(NSView, nativeUIElementPtr, space);
  if([nativeWindowPtr trackingArea]){na_ClearWindowMouseTracking(window);}
  [nativeWindowPtr setContentView:nativeUIElementPtr];
  [nativeWindowPtr setInitialFirstResponder:[nativeWindowPtr contentView]];
  
  if(window->contentSpace){naDelete(window->contentSpace);}
  window->contentSpace = space;
  na_SetUIElementParent(space, window, NA_TRUE);
  
  if([nativeWindowPtr trackingCount]){na_RenewWindowMouseTracking(window);}
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullScreen){
  naDefineCocoaObject(NACocoaNativeWindow, nativePtr, window);
  if(fullScreen != naIsWindowFullscreen(window)){
    if(fullScreen){
      window->windowedFrame = naMakeRectWithNSRect([nativePtr frame]);
      [nativePtr setStyleMask:NAWindowStyleMaskBorderless];
      [nativePtr setFrame:[[NSScreen mainScreen] frame] display:YES];
      [nativePtr setLevel:kCGScreenSaverWindowLevel];
    }else{
      [nativePtr setStyleMask:NAWindowStyleMaskTitled | NAWindowStyleMaskClosable | NAWindowStyleMaskMiniaturizable | NAWindowStyleMaskResizable];
      [nativePtr setFrame:naMakeNSRectWithRect(window->windowedFrame) display:YES];
      [nativePtr setLevel:NSNormalWindowLevel];
    }
    naSetFlagu32(&(window->coreFlags), NA_CORE_WINDOW_FLAG_FULLSCREEN, fullScreen);
    // Setting the first responder again is necessary as otherwise the first
    // responder is lost.
    [nativePtr makeFirstResponder:[nativePtr contentView]];
  }
}



NA_DEF void naSetWindowAcceptsKeyReactions(NAWindow* window, NABool accepts){
  naSetFlagu32(&(window->coreFlags), NA_CORE_WINDOW_FLAG_ACCEPTS_KEY_REACTIONS, accepts);
}


NA_HDEF NARect na_GetWindowRect(const NA_UIElement* window){
  naDefineCocoaObjectConst(NACocoaNativeWindow, nativePtr, window);
  return [nativePtr getContentRect];
}

NA_HDEF void na_SetWindowRect(NA_UIElement* window, NARect rect){
  naDefineCocoaObject(NACocoaNativeWindow, nativePtr, window);
  [nativePtr setContentRect:rect];
}

NA_DEF NARect naGetWindowOuterRect(const NAWindow* window){
  naDefineCocoaObjectConst(NACocoaNativeWindow, nativePtr, window);
  return naMakeRectWithNSRect([nativePtr frame]);
}

NA_DEF void naSetWindowOuterRect(NAWindow* window, NARect rect){
  naDefineCocoaObject(NACocoaNativeWindow, nativePtr, window);
  [nativePtr setFrame:naMakeNSRectWithRect(rect) display:YES];
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
