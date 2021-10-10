
#if defined NA_APPLICATION_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_APPLICATION_INCLUDED
#define NA_APPLICATION_INCLUDED




// ////////////////////////////////
// The Application
//
// On each system, there is at some point a so called message loop where the
// system informs NALib that the user performed an action on a ui element which
// needs handling. To get to that message loop, you start the application with
// the following function:

NA_API void naStartApplication(
  NAMutator preStartup,
  NAMutator postStartup,
  void* arg);

// All arguments can be NA_NULL but you can ask NALib to call the given two
// startup functions with the given arg. The precise order of the calls is
// described here:
//
// Mac: - NALib calls [NSApplication sharedApplication]
//      - NALib allocates some structures in the background to run the UI
//        including the application internal translator.
//      - NALib creates an NSAutoreleasePool (only when ARC is turned off)
//        * NALib calls preStartup with arg.
//        * NALib calls naResetApplicationPreferredTranslatorLanguages().
//        * NALib calls [NSApp finishLaunching] which in turn will post an
//          NSApplicationWillFinishLaunchingNotification to whatever
//          application delegate you might have set.
//        * NALib calls postStartup with arg.
//      - NALib drains the autorelease pool. (only when ARC is turned off)
//      - NALib sets its own internal application object as the apps delegate.
//      - NALib will start a message loop. When ARC is turned off, a new
//        NSAutoreleasePools is created for each and every message. At its
//        first run, a message to NSApplicationDidFinishLaunchingNotification
//        will be forwarded to whatever your application delegate was before
//        it was set to the NALib internal application object.
//
// Win: - NALib registers its window classes
//      - NALib allocates some structures in the background to run the UI
//        including the application internal translator.
//      - NALib calls preStartup with arg.
//      - NALib calls naResetApplicationPreferredTranslatorLanguages().
//      - NALib calls postStartup with arg.
//      - NALib will start a message loop.
//
// preStartup:
// The preStartup function is here for initialization of global variables
// and structures before any of the UI specific functions gets called.
// This function is intended to execute mainly C code but you of course are
// free to use other languages like Objective-C alongside with it.
//
// Note that the preStartup function is the perfect place to load your
// localizations (strings which are translated into different human languages).
// After this function, the preferred languages of the user are set in the
// translator.
//
// postStartup:
// In the postStartup function, you usually start building the UI with the
// ui elements explained below. And you add reactions to those elements which
// you would like to control. You are of course free to do this in the
// didFinishLaunching method of your NSApplication delegate on a Mac, if you
// really want to.



// ////////////////////
//
// Mixing an existing Cocoa application with NAApplication:
//
// If you require a mix of both environments, here is a very simple scheme you
// can use in your main.m file:
//
// void postStartup(void* arg){
//   [NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
//   // Now, do UI stuff with NALib.
// }
//
// int main(int argc, char *argv[]){
//   naStartRuntime();
//   [MyExistingApplication sharedApplication];
//   naStartApplication(NA_NULL, postStartup, NA_NULL);
//   return 0;
// }
//
// Notes:
// - Make sure to use the correct type for MyExistingApplication!
// - NAApplication will forward all uncaptured events unaltered.
// - You can of course add a preStartup function with translations for example.
// - Also note that the loading of nib files must sometimes be done in the
//   willFinishLaunching method of the application delegate already.
// - In order to react to willFinishLaunching and didFinishLaunching, your
//   application class must use the <NSApplicationDelegate> protocol.
//
// ////////////////////



// Note that both in the preStartup as well as the postStartup function, the
// global NAApplication struct of NALib is ready to be used. You can get this
// struct using the following call:

NA_API NAApplication* naGetApplication(void);

// If you need to get the native app pointer HINSTANCE (on Windows) or NSApp
// (on a Macintosh), call naGetUIElementNativePtr with the result.

// The message loop will run indefinitely until the application is terminated
// by a signal or it recieves a stop message using the following function: This
// Will send a stop message to the application which then in the next schedule
// will stop the run message loop and eventually return from the call
// to naStartApplication. All attached memory of the application will be freed
// and the application will not be able to run again!
NA_API void naStopApplication(void);

// Executes the given function in the given number of seconds with the given
// arg by posting a message on the main execute loop. This function returns
// immediately. Note that the timediff is given as a double.
//
// Note that this function is not defined in the NAThreading.h file because
// it only makes sense if there is a message loop which only exists when
// running a UI.
NA_API void naCallApplicationFunctionInSeconds(
  NAMutator function,
  void* arg,
  double timediff);

// Causes the translator to reset its preferred language list to the preferred
// settings of the current user. This method is called automatically during
// naStartApplication but there might be times where you want to call it
// manually.
NA_API void naResetApplicationPreferredTranslatorLanguages(void);

// Define basic information about the application. On a mac, these informations
// can be extracted automatically from a plist file if not defined. On windows
// though, one has to provide it using the following functions. Note that also
// on Mac, the settings provided here override anything else.
NA_API void naSetApplicationName(NAUTF8Char* name);
NA_API void naSetApplicationCompanyName(NAUTF8Char* name);
NA_API void naSetApplicationVersionString(NAUTF8Char* string);
NA_API void naSetApplicationBuildString(NAUTF8Char* string);
NA_API void naSetApplicationIconPath(NAUTF8Char* path);

// Retrieve the informations. All functions might return NA_NULL.
NA_API NAString* naNewApplicationName(void);
NA_API NAString* naNewApplicationCompanyName(void);
NA_API NAString* naNewApplicationVersionString(void);
NA_API NAString* naNewApplicationBuildString(void);
NA_API NAString* naNewApplicationIconPath(void);

// The application binary usually resides in some kind of base package folder
// and resources are located relative to that location. Using the following
// function, you can retrieve various informations. dir can be Null to search
// in the base package folder.
NA_API NAString* naNewApplicationResourcePath(
  const NAUTF8Char* dir,
  const NAUTF8Char* basename,
  const NAUTF8Char* suffix);



#endif // NA_APPLICATION_INCLUDED



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
