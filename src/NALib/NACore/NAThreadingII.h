
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NAThreading.h
// Do not include this file directly! It will automatically be included when
// including "NAThreading.h"



#include "../NAMemory.h"

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <windows.h>
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <unistd.h>
  #include <dispatch/dispatch.h>
  // Workaround for XCode 3 where the following macro is not defined:
  #ifndef DISPATCH_QUEUE_SERIAL
    #define DISPATCH_QUEUE_SERIAL NULL
  #endif
#endif






// ////////////////////////////
// SLEEPING
// ////////////////////////////

NA_IDEF int naSleepN(NAUInt nsecs){
  return naSleepU(nsecs / 1000);
}



NA_IDEF int naSleepU(NAUInt usecs){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    Sleep((DWORD)(usecs / 1000));
    return 0;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return usleep((useconds_t)(usecs));
  #endif
}



NA_IDEF int naSleepM(NAUInt msecs){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    Sleep((DWORD)(msecs));
    return 0;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return usleep((useconds_t)(msecs * 1000LL));
  #endif
}



NA_IDEF int naSleepS(double secs){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    Sleep((DWORD)(secs * 1000));
    return 0;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return usleep((useconds_t)(secs * 1000000LL));
  #endif
}







// ////////////////////////////
// THREADS
// ////////////////////////////

// Note that traditinally, a thred only executes one function and thus in
// some implementations, creating a thread immediately runs the thread as well.
// To stay consistent and give a higher flexibility, in NALib, the function
// to call as well as the argument is stored in a structure and the thread
// will be created when needed. Also note that with this struct, the queues
// used in Grand Central Dispatch can easily be disguised as threads. This is
// of course a strong limitation to the power of timed queues but it is the
// only way of recreating someting like threads.

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  typedef HANDLE            NANativeThread;
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  typedef dispatch_queue_t  NANativeThread;
#endif


typedef struct NAThreadStruct NAThreadStruct;
struct NAThreadStruct{
  const char* name;
  NANativeThread nativeThread;  // If you experience an error here when working
  NAMutator function;              // with plain C files on a Mac: Turn off the
  void* arg;                    // automatic reference counting in project
};                              // settings.



NA_IDEF NAThread naNewThread(const char* threadname, NAMutator function, void* arg){
  NAThreadStruct* threadstruct = naAlloc(NAThreadStruct);
  threadstruct->name = threadname;  // todo
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    threadstruct->nativeThread = NA_NULL; // Note that on windows, creating the thread would immediately start it.
  #else
    threadstruct->nativeThread = dispatch_queue_create(threadname, DISPATCH_QUEUE_SERIAL);
  #endif
  threadstruct->function = function;
  threadstruct->arg = arg;
  return threadstruct;
}



NA_IDEF void naClearThread(NAThread thread){
  NAThreadStruct* threadstruct = (NAThreadStruct*)thread;
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    CloseHandle(threadstruct->nativeThread);
  #else
    dispatch_release(threadstruct->nativeThread);
  #endif
  naFree(threadstruct);
}



#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  // Windows has a different callback type. We need to call this function first
  // in order to call our true callback function.
  NA_HDEF static DWORD __stdcall naRunWindowsThread(LPVOID arg){
    NAThreadStruct* thread = (NAThreadStruct*)arg;
    thread->function(thread->arg);
    return 0;
  }
#endif



NA_IDEF void naRunThread(NAThread thread){
  NAThreadStruct* threadstruct = (NAThreadStruct*)thread;
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    threadstruct->nativeThread = CreateThread(NULL, 0, naRunWindowsThread, threadstruct, 0, 0);
  #else
    dispatch_async_f(threadstruct->nativeThread, threadstruct->arg, threadstruct->function);
  #endif
}






// ////////////////////////////
// MUTEXES
// ////////////////////////////


#if NA_SYSTEM == NA_SYSTEM_WINDOWS

  // There are two ways to implement mutexes on windows: "Critical section" or
  // "Mutex". But as up to now, the author had some problems with the "Mutex"
  // implementation, we just use the "Critical section" which is faster anyway.
  // todo.
  #define NA_THREAD_WINDOWS_USE_CRITICAL_SECTION

  // On windows, we need a more complex structure to really make a mutex being
  // lockable just once.
  typedef struct NAWindowsMutex NAWindowsMutex;

  struct NAWindowsMutex{
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      CRITICAL_SECTION mutex;
    #else
      HANDLE mutex;
    #endif
    NABool islockedbythisthread;
    #ifndef NDEBUG
      NABool seemslocked;
    #endif
  };
  // Note that the islockedbythisthread will only be changed by the thread
  // which has locked the mutex and therefore will only evaluate to TRUE, when
  // the thread already has locked it. This is how we detect if the current
  // thread makes a double lock. The native "Threads" and "Critical sections"
  // of Windows do not do that. They allow mutliple locking as long as it is
  // the same thread. The author thinks that this is inconsistent and therefore
  // has implemented mutexes like this to be the same on all systems.

#else

  #ifndef NDEBUG
    typedef struct NAMacintoshMutex NAMacintoshMutex;
    struct NAMacintoshMutex{
      dispatch_semaphore_t mutex;
      NABool seemslocked;
    };
  #else
    typedef dispatch_semaphore_t NAMacintoshMutex;
  #endif


#endif



NA_IDEF NAMutex naNewMutex(){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = naAlloc(NAWindowsMutex);
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      InitializeCriticalSection(&(windowsmutex->mutex));
    #else
      windowsmutex->mutex = CreateMutex(NULL, FALSE, NULL);
    #endif
    windowsmutex->islockedbythisthread = NA_FALSE;
    #ifndef NDEBUG
      windowsmutex->seemslocked = NA_FALSE;
    #endif
    return windowsmutex;
  #else
  
    #ifndef NDEBUG
      NAMacintoshMutex* macintoshmutex = naAlloc(NAMacintoshMutex);
      macintoshmutex->mutex = dispatch_semaphore_create(1);
      macintoshmutex->seemslocked = NA_FALSE;
      return macintoshmutex;
    #else
      NAMutex mutex = dispatch_semaphore_create(1);
      return mutex;
    #endif
  #endif
}



NA_IDEF void naClearMutex(NAMutex mutex){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = (NAWindowsMutex*)mutex;
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      DeleteCriticalSection(&(windowsmutex->mutex));
    #else
      CloseHandle(windowsmutex->mutex);
    #endif
    naFree(windowsmutex);
  #else
    #ifndef NDEBUG
      NAMacintoshMutex* macintoshmutex = (NAMacintoshMutex*)mutex;
      dispatch_release(macintoshmutex->mutex);
      naFree(macintoshmutex);
    #else
      dispatch_release(mutex);
    #endif
  #endif
}




NA_IDEF void naLockMutex(NAMutex mutex){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = (NAWindowsMutex*)mutex;
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      EnterCriticalSection(&(windowsmutex->mutex));
    #else
      WaitForSingleObject(windowsmutex->mutex, INFINITE);
    #endif
    #ifndef NDEBUG
      if(windowsmutex->islockedbythisthread)
        naError("naLockMutex", "Mutex was already locked by this thread. This is not how Mutexes in NALib work.");
    #endif
    windowsmutex->islockedbythisthread = NA_TRUE;
    #ifndef NDEBUG
      windowsmutex->seemslocked = NA_TRUE;
    #endif
   #else
    #ifndef NDEBUG
      NAMacintoshMutex* macintoshmutex = (NAMacintoshMutex*)mutex;
      dispatch_semaphore_wait(macintoshmutex->mutex, DISPATCH_TIME_FOREVER);
      macintoshmutex->seemslocked = NA_TRUE;
    #else
      dispatch_semaphore_wait(mutex, DISPATCH_TIME_FOREVER);
    #endif
  #endif
}



NA_IDEF void naUnlockMutex(NAMutex mutex){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = (NAWindowsMutex*)mutex;
    #ifndef NDEBUG
      if(!naIsMutexLocked(mutex))
        naError("naUnlockMutex", "Mutex was not locked. Note: If this only happends once and very rarely, it might be because this check is unreliable!");
    #endif
    #ifndef NDEBUG
      windowsmutex->seemslocked = NA_FALSE;
    #endif
    windowsmutex->islockedbythisthread = NA_FALSE;
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      LeaveCriticalSection(&(windowsmutex->mutex));
    #else
      ReleaseMutex(windowsmutex->mutex);
    #endif
  #else
    #ifndef NDEBUG
      NAMacintoshMutex* macintoshmutex = (NAMacintoshMutex*)mutex;
      macintoshmutex->seemslocked = NA_FALSE;
      dispatch_semaphore_signal(macintoshmutex->mutex);
    #else
      dispatch_semaphore_signal(mutex);
    #endif
  #endif
}



#ifndef NDEBUG
  NA_IDEF NABool naIsMutexLocked(NAMutex mutex){
    #if NA_SYSTEM == NA_SYSTEM_WINDOWS
      NAWindowsMutex* windowsmutex = (NAWindowsMutex*)mutex;
      return windowsmutex->seemslocked;
    #else
      NAMacintoshMutex* macintoshmutex = (NAMacintoshMutex*)mutex;
      return macintoshmutex->seemslocked;
    #endif
  
    // This was the old code which worked but it introduced long wait times
    // when debugging. As this function is unreliable anyway, the whole test
    // whether being locked or not has been put into a debug flag in a
    // special type only available when debugging.
    //
    //  NABool hasjustbeenlocked = naTryMutex(mutex);
    //  if(hasjustbeenlocked){
    //    naUnlockMutex(mutex);
    //    return NA_FALSE;
    //  }else{
    //    return NA_TRUE;
    //  }
  }
#endif



NA_IDEF NABool naTryMutex(NAMutex mutex){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = (NAWindowsMutex*)mutex;
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      BOOL retvalue = TryEnterCriticalSection(&(windowsmutex->mutex));
      if(retvalue == 0){
        return NA_FALSE;
      }else{
        if(windowsmutex->islockedbythisthread){
          LeaveCriticalSection(&(windowsmutex->mutex));
          return NA_FALSE;
        }else{
          windowsmutex->islockedbythisthread = NA_TRUE;
          return NA_TRUE;
        }
      }
    #else
      DWORD retvalue = WaitForSingleObject(windowsmutex->mutex, 0);
      if(retvalue == WAIT_OBJECT_0){
        return NA_FALSE;
      }else{
        // somehow, this does not work yet. use Critical section.
        if(windowsmutex->islockedbythisthread){
          ReleaseMutex(windowsmutex->mutex);
          return NA_FALSE;
        }else{
          windowsmutex->islockedbythisthread = NA_TRUE;
          return NA_TRUE;
        }
      }
    #endif
  #else
    #ifndef NDEBUG
      NAMacintoshMutex* macintoshmutex = (NAMacintoshMutex*)mutex;
      long retvalue = dispatch_semaphore_wait(macintoshmutex->mutex, DISPATCH_TIME_NOW);
      if(retvalue){
        return NA_FALSE;
      }else{
        return NA_TRUE;
        macintoshmutex->seemslocked = NA_TRUE;
      }
    #else
      long retvalue = dispatch_semaphore_wait(mutex, DISPATCH_TIME_NOW);
      return (retvalue ? NA_FALSE : NA_TRUE);
    #endif
  #endif
}







// ////////////////////////////
// ALARMS
// ////////////////////////////


#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  typedef HANDLE            NANativeAlarm;
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  typedef dispatch_semaphore_t  NANativeAlarm;
#endif



NA_IDEF NAAlarm naNewAlarm(){
  NANativeAlarm alarm;
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    alarm = CreateEvent(NULL, FALSE, FALSE, NULL);
  #else
    alarm = dispatch_semaphore_create(0);
  #endif
  return alarm;
}



NA_IDEF void naClearAlarm(NAAlarm alarm){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    CloseHandle(alarm);
  #else
    dispatch_release(alarm);
  #endif
}



NA_IDEF NABool naAwaitAlarm(NAAlarm alarm, double maxwaittime){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    DWORD result;
    #ifndef NDEBUG
      if(maxwaittime < 0.)
        naError("naAwaitAlarm", "maxwaittime should not be negative. Beware of the zero!");
    #endif
    ResetEvent(alarm);
    if(maxwaittime == 0){
      result = WaitForSingleObject(alarm, INFINITE);
    }else{
      result = WaitForSingleObject(alarm, (DWORD)(1000. * maxwaittime));
    }
    return (result == WAIT_OBJECT_0);
  #else
    long result;
    #ifndef NDEBUG
      if(maxwaittime < 0.)
        naError("naAwaitAlarm", "maxwaittime should not be negative. Beware of the zero!");
    #endif
    if(maxwaittime == 0){
      result = dispatch_semaphore_wait(alarm, DISPATCH_TIME_FOREVER);
    }else{
      dispatch_time_t timeout = dispatch_time(DISPATCH_TIME_NOW, 1000000000. * maxwaittime);
      result = dispatch_semaphore_wait(alarm, timeout);
    }
    return (result ? NA_FALSE : NA_TRUE);
  #endif
}




NA_IDEF void naTriggerAlarm(NAAlarm alarm){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    SetEvent(alarm);
  #else
    dispatch_semaphore_signal(alarm);
  #endif
}



// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
