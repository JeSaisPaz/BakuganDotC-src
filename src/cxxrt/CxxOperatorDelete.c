// bdc 0x08a03054 CxxOperatorDelete
#include "bdc.h"

/* `operator delete(void *)`: `free` of a non-NULL pointer inside a no-throw frame. */

void CxxOperatorDelete(void *ptr)
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
  if (ptr != (void *)0x0) {
    free(ptr);
  }
  g_cxxEhFrameStack = frame.next;
}
