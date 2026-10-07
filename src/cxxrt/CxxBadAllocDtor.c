// bdc 0x08a32818 CxxBadAllocDtor
#include "bdc.h"

/* Destructor of `std::bad_alloc` (vtable `0x08af7068` entry 1, thrown by `CxxThrowBadAlloc`):
   inside an EH cleanup frame, reinstalls the vtable, runs `CxxExceptionDtor` and frees the object
   (`CxxOperatorDelete`) when `flags & 1`. */

void CxxBadAllocDtor(void *exc, u32 flags)

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
  ehCleanupDesc = &g_cxxBadAllocDtorCleanup;
  savedRegion = g_cxxEhCurrentRegion;
  if (exc != (void *)0x0) {
    *(void ***)exc = g_cxxBadAllocVtable;
    g_cxxEhCurrentRegion = 0xffff;
    ehObj = exc;
    CxxExceptionDtor(exc,0);
    if ((flags & 1) != 0) {
      CxxOperatorDelete(exc);
    }
  }
  g_cxxEhCurrentRegion = savedRegion;
  g_cxxEhFrameStack = ehNext;
  return;
}

