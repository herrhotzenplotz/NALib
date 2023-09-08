
#if defined NA_KEYBOARD_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_KEYBOARD_INCLUDED
#define NA_KEYBOARD_INCLUDED



// When handling keyboard input, NALib captures the keys pressed and provides
// you commands for KeyDown and KeyUp. The key pressed is stored in the
// following type:
typedef size_t NAUIKeyCode;

// A NAUIKeyCode can have the following values. These values correspond to the
// "US extended" keyboard layout. They represent the physical keys and not the
// characters they might result in.
#if NA_OS == NA_OS_WINDOWS
  #define NA_KEYCODE_0              0x0b
  #define NA_KEYCODE_1              0x02
  #define NA_KEYCODE_2              0x03
  #define NA_KEYCODE_3              0x04
  #define NA_KEYCODE_4              0x05
  #define NA_KEYCODE_5              0x06
  #define NA_KEYCODE_6              0x07
  #define NA_KEYCODE_7              0x08
  #define NA_KEYCODE_8              0x09
  #define NA_KEYCODE_9              0x0a
  #define NA_KEYCODE_A              0x1e
  #define NA_KEYCODE_B              0x30
  #define NA_KEYCODE_C              0x2e
  #define NA_KEYCODE_D              0x20
  #define NA_KEYCODE_E              0x12
  #define NA_KEYCODE_F              0x21
  #define NA_KEYCODE_G              0x22
  #define NA_KEYCODE_H              0x23
  #define NA_KEYCODE_I              0x17
  #define NA_KEYCODE_J              0x24
  #define NA_KEYCODE_K              0x25
  #define NA_KEYCODE_L              0x26
  #define NA_KEYCODE_M              0x32
  #define NA_KEYCODE_N              0x31
  #define NA_KEYCODE_O              0x18
  #define NA_KEYCODE_P              0x19
  #define NA_KEYCODE_Q              0x10
  #define NA_KEYCODE_R              0x13
  #define NA_KEYCODE_S              0x1f
  #define NA_KEYCODE_T              0x14
  #define NA_KEYCODE_U              0x16
  #define NA_KEYCODE_V              0x2f
  #define NA_KEYCODE_W              0x11
  #define NA_KEYCODE_X              0x2d
  #define NA_KEYCODE_Y              0x15
  #define NA_KEYCODE_Z              0x2c
  #define NA_KEYCODE_SPACE          0x39
  #define NA_KEYCODE_TAB            0x0f
  #define NA_KEYCODE_ENTER          0x1c
  #define NA_KEYCODE_LEFT_SHIFT     0x2a
  #define NA_KEYCODE_RIGHT_SHIFT    0x2a // right shift is not captured like this
  #define NA_KEYCODE_LEFT_CONTROL   0x1d
  #define NA_KEYCODE_RIGHT_CONTROL  0x1d
  #define NA_KEYCODE_LEFT_OPTION    0x38 // Handeled differently on windows
  #define NA_KEYCODE_RIGHT_OPTION   0x38 // Handeled differently on windows
  #define NA_KEYCODE_LEFT_COMMAND   0x5b // The windows key
  #define NA_KEYCODE_RIGHT_COMMAND  0x5c
  #define NA_KEYCODE_FN             0xff // Can not catch.
  #define NA_KEYCODE_ESC            0x01
  #define NA_KEYCODE_F11            0x57
  #define NA_KEYCODE_MINUS          0x0c
  #define NA_KEYCODE_EQUAL          0x0d
  #define NA_KEYCODE_PERIOD         0x34
  #define NA_KEYCODE_NUMPAD_MINUS   0x4a
  #define NA_KEYCODE_NUMPAD_PLUS    0x4e
  #define NA_KEYCODE_NUMPAD_ENTER   0x1c  // can not distinguish from ENTER
  #define NA_KEYCODE_ARROW_LEFT     0x4b
  #define NA_KEYCODE_ARROW_RIGHT    0x4d
  #define NA_KEYCODE_ARROW_DOWN     0x50
  #define NA_KEYCODE_ARROW_UP       0x48
#elif NA_OS == NA_OS_MAC_OS_X
  #define NA_KEYCODE_0              0x1d
  #define NA_KEYCODE_1              0x12
  #define NA_KEYCODE_2              0x13
  #define NA_KEYCODE_3              0x14
  #define NA_KEYCODE_4              0x15
  #define NA_KEYCODE_5              0x17
  #define NA_KEYCODE_6              0x16
  #define NA_KEYCODE_7              0x1a
  #define NA_KEYCODE_8              0x1c
  #define NA_KEYCODE_9              0x19
  #define NA_KEYCODE_A              0x00
  #define NA_KEYCODE_B              0x0b
  #define NA_KEYCODE_C              0x08
  #define NA_KEYCODE_D              0x02
  #define NA_KEYCODE_E              0x0e
  #define NA_KEYCODE_F              0x03
  #define NA_KEYCODE_G              0x05
  #define NA_KEYCODE_H              0x04
  #define NA_KEYCODE_I              0x22
  #define NA_KEYCODE_J              0x26
  #define NA_KEYCODE_K              0x28
  #define NA_KEYCODE_L              0x25
  #define NA_KEYCODE_M              0x2e
  #define NA_KEYCODE_N              0x2d
  #define NA_KEYCODE_O              0x1f
  #define NA_KEYCODE_P              0x23
  #define NA_KEYCODE_Q              0x0c
  #define NA_KEYCODE_R              0x0f
  #define NA_KEYCODE_S              0x01
  #define NA_KEYCODE_T              0x11
  #define NA_KEYCODE_U              0x20
  #define NA_KEYCODE_V              0x09
  #define NA_KEYCODE_W              0x0d
  #define NA_KEYCODE_X              0x07
  #define NA_KEYCODE_Y              0x10
  #define NA_KEYCODE_Z              0x06
  #define NA_KEYCODE_SPACE          0x31
  #define NA_KEYCODE_TAB            0x30
  #define NA_KEYCODE_ENTER          0x24
  #define NA_KEYCODE_LEFT_SHIFT     0x38
  #define NA_KEYCODE_RIGHT_SHIFT    0x3c
  #define NA_KEYCODE_CONTROL        0x3b
  #define NA_KEYCODE_LEFT_OPTION    0x3a
  #define NA_KEYCODE_RIGHT_OPTION   0x3d
  #define NA_KEYCODE_LEFT_COMMAND   0x37
  #define NA_KEYCODE_RIGHT_COMMAND  0x36
  #define NA_KEYCODE_FN             0x3f
  #define NA_KEYCODE_ESC            0x35
  #define NA_KEYCODE_F11            0x67
  #define NA_KEYCODE_MINUS          0x1b
  #define NA_KEYCODE_EQUAL          0x18
  #define NA_KEYCODE_PERIOD         0x2f
  #define NA_KEYCODE_NUMPAD_MINUS   0x4e
  #define NA_KEYCODE_NUMPAD_PLUS    0x45
  #define NA_KEYCODE_NUMPAD_ENTER   0x4c
  #define NA_KEYCODE_ARROW_LEFT     0x7b
  #define NA_KEYCODE_ARROW_RIGHT    0x7c
  #define NA_KEYCODE_ARROW_DOWN     0x7d
  #define NA_KEYCODE_ARROW_UP       0x7e
#endif

// The modifier keys are stored as a bitmask with the following predefined
// elements:
typedef enum{
  NA_MODIFIER_FLAG_NONE          = 0x0000,
  NA_MODIFIER_FLAG_SHIFT         = 0x0003,
  NA_MODIFIER_FLAG_CONTROL       = 0x000c,
  NA_MODIFIER_FLAG_OPTION        = 0x0030,
  NA_MODIFIER_FLAG_COMMAND       = 0x00c0

  // The following flags will be available in the future. Please be patient.
//  NA_MODIFIER_FLAG_LEFT_SHIFT    = 0x0001,
//  NA_MODIFIER_FLAG_RIGHT_SHIFT   = 0x0002,
//  NA_MODIFIER_FLAG_LEFT_CONTROL  = 0x0004,
//  NA_MODIFIER_FLAG_RIGHT_CONTROL = 0x0008,
//  NA_MODIFIER_FLAG_LEFT_OPTION   = 0x0010,
//  NA_MODIFIER_FLAG_RIGHT_OPTION  = 0x0020,
//  NA_MODIFIER_FLAG_LEFT_COMMAND  = 0x0040,
//  NA_MODIFIER_FLAG_RIGHT_COMMAND = 0x0080,
} NAModifierFlag;



// A key stroke stores a key combination consisting of modifier flags and a
// single key.
typedef struct NAKeyStroke NAKeyStroke;
struct NAKeyStroke{
  uint32 modifiers;
  NAUIKeyCode keyCode;
};



// Returns the currently pressed key combination
NA_API NAKeyStroke naGetCurrentKeyStroke(void);

// Returns a key stroke which depicts a single key pressed in combination with
// modifier keys. Can for example be used as input for naAddUIKeyboardShortcut.
NA_API NAKeyStroke naMakeKeyStroke(uint32 modifiers, NAUIKeyCode keyCode);

// Returns a string which represents what the user would see when pressing
// the given key with the given modifiers. Implemented system-specific.
NAString* naNewKeyPressString(uint32 modifiers, NAUIKeyCode keyCode);

#endif // NA_KEYBOARD_INCLUDED



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
