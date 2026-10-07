// bdc 0x08a03210 CxxBadExceptionDtor
#include "bdc.h"

/* Destructor of `std::bad_exception`: restores its vtable, runs `CxxExceptionDtor` and frees the
   object when `flags & 1`. */

void CxxBadExceptionDtor(void *self, u32 flags)

{
  void *ehObj;
  void *ehNext;
  u8 ehKind;
  u32 ehPad;
  void **ehFrame;
  u8 ehCleanupKind;
  void **ehCleanupDesc;
  u8 *ehObjSlot;
  u16 savedRegion;
  
  ehObjSlot = (u8 *)&ehObj;
  ehFrame = &ehNext;
  ehNext = g_cxxEhFrameStack;
  ehKind = 2;
  ehPad = 0;
  g_cxxEhFrameStack = &ehFrame;
  ehCleanupKind = 1;
  ehCleanupDesc = &g_cxxBadExceptionDtorCleanup;
  savedRegion = g_cxxEhCurrentRegion;
  if (self != (void *)0x0) {
    *(void ***)self = g_cxxBadExceptionVtable;
    g_cxxEhCurrentRegion = 0xffff;
    ehObj = self;
    CxxExceptionDtor(self,0);
    if ((flags & 1) != 0) {
      CxxOperatorDelete(self);
    }
  }
  g_cxxEhCurrentRegion = savedRegion;
  g_cxxEhFrameStack = ehNext;
  return;
}

