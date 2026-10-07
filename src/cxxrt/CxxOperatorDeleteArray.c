// bdc 0x08a02fc4 CxxOperatorDeleteArray
#include "bdc.h"

/* `operator delete[](void *)`: forwards to `operator delete` (`CxxOperatorDelete`) inside a
   no-throw frame. */

void CxxOperatorDeleteArray(void *ptr)
{
  struct {
    void *next;
    u8 kind;
    u32 spec;
  } frame;

  frame.next = g_cxxEhFrameStack;
  frame.kind = 2;
  g_cxxEhFrameStack = &frame;
  frame.spec = 0;
  CxxOperatorDelete(ptr);
  g_cxxEhFrameStack = frame.next;
}
