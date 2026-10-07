// bdc 0x08a03b94 CxxEhPushThrowFrame
#include "bdc.h"

/* Links the frame embedded in the current exception record (`+0x34`) onto the exception-handling
   frame stack (`g_cxxEhFrameStack`, frames `{next, u8 kind, ...}`: 0 try block, 1 cleanup region,
   2 exception specification, 3 no-throw, 4 array construction) and marks it pushed (`+0x30`). */

void CxxEhPushThrowFrame(void)

{
  CxxEhRecord *rec = (CxxEhRecord *)g_cxxEhCurrentException;

  rec->frame.next = (CxxEhFrame *)g_cxxEhFrameStack;
  g_cxxEhFrameStack = &rec->frame;
  rec->pushed = 1;
}
