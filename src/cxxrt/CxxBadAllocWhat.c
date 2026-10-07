// bdc 0x08a32800 CxxBadAllocWhat
#include "bdc.h"

/* `std::bad_alloc::what()` (entry after `CxxBadAllocDtor` in the `bad_alloc` vtable
   `0x08af7068`): returns `"bad_alloc"`. */

const char *CxxBadAllocWhat(void *self)

{
  g_cxxEhFrameStack = g_cxxEhFrameStack;
  return "bad_alloc";
}

