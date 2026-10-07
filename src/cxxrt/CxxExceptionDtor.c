// bdc 0x08a030e0 CxxExceptionDtor
#include "bdc.h"

/* Destructor of `std::exception` (vtable `0x08af5a80` slot 1): inside an empty exception-specification
   frame (no exception may escape), restores the vtable and frees the object (`CxxOperatorDelete`)
   when `flags & 1`; a NULL `self` does nothing. */

void CxxExceptionDtor(void *self, u32 flags)
{
  CxxEhSpecFrame frame;

  frame.next = (CxxEhSpecFrame *)g_cxxEhFrameStack;
  g_cxxEhFrameStack = &frame;
  frame.kind = 2;
  frame.list = NULL;
  if (self != NULL) {
    *(void ***)self = g_cxxStdExceptionVtable;
    if ((flags & 1) != 0) {
      CxxOperatorDelete(self);
    }
  }
  g_cxxEhFrameStack = frame.next;
}
