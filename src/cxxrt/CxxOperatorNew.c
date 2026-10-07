// bdc 0x08a03304 CxxOperatorNew
#include "bdc.h"

/* `operator new(size_t)`: `malloc``(size ? size : 1)` in a loop; on failure calls the installed
   new-handler (`g_cxxNewHandler`) or the default `CxxThrowBadAlloc` and retries. Runs inside a
   `throw(std::bad_alloc)` specification frame. */

void *CxxOperatorNew(u32 size)
{
  struct {
    void *next;
    u8 kind;
    void *spec;
  } frame;
  void *result;
  void (*handler)(void);

  frame.next = g_cxxEhFrameStack;
  frame.kind = 2;
  g_cxxEhFrameStack = &frame;
  frame.spec = &g_cxxSpecBadAllocB;
  if (size == 0) {
    size = 1;
  }
  while ((result = malloc(size)) == (void *)0x0) {
    handler = (void (*)(void))g_cxxNewHandler;
    if (handler == (void (*)(void))0x0) {
      handler = CxxThrowBadAlloc;
    }
    handler();
  }
  g_cxxEhFrameStack = frame.next;
  return result;
}
