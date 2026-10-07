// bdc 0x08a03008 CxxOperatorNewArray
#include "bdc.h"

/* `operator new[](size_t)`: forwards to `operator new` (`CxxOperatorNew`) inside an
   exception-specification frame (`throw(std::bad_alloc)`, spec `g_cxxSpecBadAllocA`). Default
   allocator of `CxxVecNewEx`. */

void *CxxOperatorNewArray(u32 size)
{
  struct {
    void *next;
    u8 kind;
    void *spec;
  } frame;
  void *result;

  frame.next = g_cxxEhFrameStack;
  frame.kind = 2;
  g_cxxEhFrameStack = &frame;
  frame.spec = &g_cxxSpecBadAllocA;
  result = CxxOperatorNew(size);
  g_cxxEhFrameStack = frame.next;
  return result;
}
