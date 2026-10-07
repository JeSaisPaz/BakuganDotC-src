// bdc 0x08a043a0 CxxCallUnexpected
#include "bdc.h"

/* Handles an exception that violates an exception specification: inside its own cleanup frame and
   try block (setjmp `0x08a0f78c`, handler list `g_cxxCallUnexpectedHandlers`) calls
   `unexpected()` (`CxxUnexpected`); if that throws a type the specification allows, rethrows it
   (`CxxRethrow`); else, if `std::bad_exception` (`g_cxxBadExceptionTypeInfo`) is allowed, throws
   one (`CxxBadExceptionCtor`, `CxxThrow`); otherwise terminates (`CxxTerminateInternal`).
   Every path that comes back ends in `abort`; never returns. */

void CxxCallUnexpected(void)

{
  CxxEhTryFrame tryFrame;
  CxxEhCleanupFrame cleanup;
  void *type;
  u8 flags[4];
  u32 extra;
  u16 savedRegion;

  savedRegion = g_cxxEhCurrentRegion;
  cleanup.next = (CxxEhFrame *)g_cxxEhFrameStack;
  cleanup.kind = 1;
  g_cxxEhCurrentRegion = 0xffff;
  cleanup.region = savedRegion;
  tryFrame.next = (CxxEhFrame *)&cleanup;
  g_cxxEhFrameStack = &tryFrame;
  tryFrame.kind = 0;
  tryFrame.handlers = &g_cxxCallUnexpectedHandlers;
  tryFrame.caught = NULL;
  tryFrame.region = g_cxxEhCurrentRegion;
  if (setjmp(tryFrame.jmpBuf) == 0) {
    CxxUnexpected();
  }
  else {
    CxxEhPopFrame();
    CxxEhGetCurrentType(&type, flags, &extra);
    if (CxxEhSpecAllows(type, flags[0], extra) != 0) {
      CxxRethrow();
    }
    else if (CxxEhSpecAllows(&g_cxxBadExceptionTypeInfo, 0, 0) != 0) {
      CxxBadExceptionCtor(CxxEhAllocException(&g_cxxBadExceptionTypeInfo, 4, 0));
      CxxThrow();
    }
    else {
      CxxTerminateInternal();
    }
    CxxEhEndCatch();
  }
  g_cxxEhFrameStack = tryFrame.next;
  abort();
  g_cxxEhCurrentRegion = cleanup.region;
  g_cxxEhFrameStack = cleanup.next;
  for (;;) {
  }
}
