// bdc 0x08a03158 CxxBadExceptionCtor
#include "bdc.h"

/* Constructor of `std::bad_exception` (vtable `0x08af5a98`, type info `0x08af5ac4`): pushes a
   kind-2 frame and a cleanup frame (descriptor `g_cxxBadExceptionCtorCleanup`), runs
   `CxxExceptionCtor` with no current region, records `self` as the constructed object, installs
   the derived vtable, then restores the region and pops both frames. Returns `self`. Used by
   `CxxCallUnexpected`. */

void *CxxBadExceptionCtor(void *self)

{
  CxxEhFrame ctorFrame;
  u32 ctorFramePad;
  CxxEhCleanupFrame cleanup;
  void *ehObj;

  ctorFrame.next = (CxxEhFrame *)g_cxxEhFrameStack;
  g_cxxEhFrameStack = &ctorFrame;
  ctorFrame.kind = 2;
  ctorFramePad = 0;
  cleanup.next = &ctorFrame;
  g_cxxEhFrameStack = &cleanup;
  cleanup.kind = 1;
  cleanup.entries = (CxxEhCleanupEntry *)&g_cxxBadExceptionCtorCleanup;
  cleanup.objects = &ehObj;
  cleanup.region = g_cxxEhCurrentRegion;
  g_cxxEhCurrentRegion = 0xffff;
  CxxExceptionCtor(self);
  g_cxxEhCurrentRegion = 0;
  ehObj = self;
  *(void ***)self = g_cxxBadExceptionVtable;
  g_cxxEhCurrentRegion = cleanup.region;
  g_cxxEhFrameStack = ctorFrame.next;
  return self;
}
