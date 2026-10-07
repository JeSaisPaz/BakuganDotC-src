// bdc 0x08a03bbc CxxEhPopFrame
#include "bdc.h"

/* Pops the top frame of the exception-handling frame stack (`g_cxxEhFrameStack`, frames `{next,
   u8 kind, ...}`: 0 try block, 1 cleanup region, 2 exception specification, 3 no-throw, 4 array
   construction). */

void CxxEhPopFrame(void)

{
  g_cxxEhFrameStack = *(void **)g_cxxEhFrameStack;
  return;
}

