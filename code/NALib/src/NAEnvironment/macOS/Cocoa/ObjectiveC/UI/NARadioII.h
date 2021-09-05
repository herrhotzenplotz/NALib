
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NACocoaRadio NACocoaRadio;
struct NACocoaRadio{
  NARadio radio;
};

NA_HAPI void na_DestructCocoaRadio(NACocoaRadio* cocoaRadio);
NA_RUNTIME_TYPE(NACocoaRadio, na_DestructCocoaRadio, NA_FALSE);

@interface NACocoaNativeRadio : NSButton <NACocoaNativeEncapsulatedElement>{
  NACocoaRadio* cocoaRadio;
  NSView*       containingView;
}
- (NSView*) getEncapsulatingView;
@end



@implementation NACocoaNativeRadio

- (id) initWithRadio:(NACocoaRadio*)newCocoaRadio frame:(NSRect)frame{
  NSRect newbounds = frame;
  newbounds.origin.x = 0;
  newbounds.origin.y = 0;

  self = [super initWithFrame:newbounds];
  
  [self setButtonType:NAButtonTypeRadio];
//  [self setBezelStyle:NSBezelStyleRounded];
//  [self setBezelStyle:NSBezelStyleShadowlessSquare];
//  [self setBordered:YES];
  cocoaRadio = newCocoaRadio;
  [self setTarget:self];
  [self setAction:@selector(onPressed:)];
  [self setFont:[NSFont labelFontOfSize:[NSFont systemFontSize]]];

  containingView = [[NSView alloc] initWithFrame:frame];
  [containingView addSubview:self];

  return self;
}

- (void)dealloc{
  NA_COCOA_RELEASE(containingView);
  NA_COCOA_SUPER_DEALLOC();
}

- (NSView*) getEncapsulatingView{
  return containingView;
}

- (void) setText:(const NAUTF8Char*)text{
  [self setTitle:[NSString stringWithUTF8String:text]];
}

- (void) setColor:(const NABabyColor*)color{
  NSColor* nsColor;
  if(color){
    uint8 buf[4];
    naFillu8WithBabyColor(buf, *color, NA_COLOR_BUFFER_RGBA);
    nsColor = [NSColor colorWithCalibratedRed:buf[0] / 255. green:buf[1] / 255. blue:buf[2] / 255. alpha:buf[3] / 255.];
  }else{
    nsColor = naGetLabelColor();
  }
  NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithAttributedString:[self attributedTitle]];
  NSRange range = NSMakeRange(0, [attrString length]);

  [attrString beginEditing];
//  NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
//  [paragraphStyle setParagraphStyle:[NSParagraphStyle defaultParagraphStyle]];
//  paragraphStyle.alignment = [self alignment];
//  NA_COCOA_RELEASE(paragraphStyle);
  [attrString addAttribute:NSForegroundColorAttributeName value:nsColor range:range];
  [attrString endEditing];
  
  [self setAttributedTitle: attrString];
  NA_COCOA_RELEASE(attrString);
}

- (void) onPressed:(id)sender{
  NA_UNUSED(sender);
  na_DispatchUIElementCommand((NA_UIElement*)cocoaRadio, NA_UI_COMMAND_PRESSED);
}

- (void) setRadioState:(NABool)state{
  [self setState:state ? NAStateOn : NAStateOff];
}

- (NABool) radioState{
  return ([self state] == NAStateOn) ? NA_TRUE : NA_FALSE;
}

- (NARect) getInnerRect{
  return naMakeRectWithNSRect([self frame]);
}

@end



NA_DEF NARadio* naNewRadio(const NAUTF8Char* text, NASize size){
  NACocoaRadio* cocoaRadio = naNew(NACocoaRadio);

  NACocoaNativeRadio* nativePtr = [[NACocoaNativeRadio alloc]
    initWithRadio:cocoaRadio
    frame:naMakeNSRectWithSize(size)];
  na_InitRadio((NARadio*)cocoaRadio, NA_COCOA_PTR_OBJC_TO_C(nativePtr));
  
  [nativePtr setText:text];
  
  return (NARadio*)cocoaRadio;
}



NA_DEF void na_DestructCocoaRadio(NACocoaRadio* cocoaRadio){
  na_ClearRadio((NARadio*)cocoaRadio);
}



NA_DEF void naSetRadioTextColor(NARadio* radio, const NABabyColor* color){
  naDefineCocoaObject(NACocoaNativeRadio, nativePtr, radio);
  [nativePtr setColor:color];
}



NA_HDEF NARect na_GetRadioAbsoluteInnerRect(NA_UIElement* radio){
  naDefineCocoaObject(NACocoaNativeRadio, nativePtr, radio);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(radio), naGetApplication(), NA_FALSE);
  NARect relRect = [nativePtr getInnerRect];
  return naMakeRect(
    naMakePos(parentRect.pos.x + relRect.pos.x, parentRect.pos.y + relRect.pos.y),
    relRect.size);
}



NA_DEF void naSetRadioState(NARadio* radio, NABool state){
  naDefineCocoaObject(NACocoaNativeRadio, nativePtr, radio);
  [nativePtr setRadioState:state];
}



NA_DEF void naSetRadioEnabled(NARadio* radio, NABool enabled){
  naDefineCocoaObject(NACocoaNativeRadio, nativePtr, radio);
  [nativePtr setEnabled:(BOOL)enabled];
}



NA_DEF NABool naGetRadioState(NARadio* radio){
  naDefineCocoaObject(NACocoaNativeRadio, nativePtr, radio);
  return [nativePtr radioState];
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
