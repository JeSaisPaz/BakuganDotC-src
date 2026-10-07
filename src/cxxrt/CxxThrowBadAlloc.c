// bdc 0x08a033b0 CxxThrowBadAlloc
#include "bdc.h"

/* Default new-handler: allocates an exception object of type `std::bad_alloc`
   (`CxxEhAllocException`, type `g_cxxBadAllocTypeInfo`) and, inside an empty `throw()`
   specification frame and a cleanup frame that destroys it on unwind (`g_cxxExceptionDtorCleanup`),
   constructs it (`CxxExceptionCtor` + vtable `g_cxxBadAllocVtable`); then pops both frames and
   throws it (`CxxThrow`). Does not return. */

void CxxThrowBadAlloc(void)

{
  void *object[1];
  CxxEhSpecFrame spec;
  CxxEhCleanupFrame cleanup;
  void **self;

  self = CxxEhAllocException(&g_cxxBadAllocTypeInfo, 4, 0);
  spec.next = (CxxEhSpecFrame *)g_cxxEhFrameStack;
  g_cxxEhFrameStack = &spec;
  spec.kind = 2;
  spec.list = NULL;
  cleanup.next = (CxxEhFrame *)&spec;
  g_cxxEhFrameStack = &cleanup;
  cleanup.kind = 1;
  cleanup.entries = (CxxEhCleanupEntry *)&g_cxxExceptionDtorCleanup;
  cleanup.objects = object;
  cleanup.region = g_cxxEhCurrentRegion;
  g_cxxEhCurrentRegion = 0xffff;
  CxxExceptionCtor(self);
  object[0] = self;
  g_cxxEhCurrentRegion = 0;
  *self = g_cxxBadAllocVtable;
  g_cxxEhCurrentRegion = cleanup.region;
  g_cxxEhFrameStack = spec.next;
  CxxThrow();
}
