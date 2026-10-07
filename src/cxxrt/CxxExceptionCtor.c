// bdc 0x08a030a0 CxxExceptionCtor
#include "bdc.h"

/* Constructor of `std::exception`: inside an empty exception-specification frame (no exception may
   escape), installs its vtable `0x08af5a80` (type info `0x08af5ab0`, name `"std::exception"`).
   Returns `self`. */

void *CxxExceptionCtor(void *self)
{
  CxxEhSpecFrame frame;

  frame.next = (CxxEhSpecFrame *)g_cxxEhFrameStack;
  g_cxxEhFrameStack = &frame;
  frame.kind = 2;
  frame.list = NULL;
  *(void ***)self = g_cxxStdExceptionVtable;
  g_cxxEhFrameStack = frame.next;
  return self;
}
