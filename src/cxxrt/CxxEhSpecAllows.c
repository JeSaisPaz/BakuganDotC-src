// bdc 0x08a03eec CxxEhSpecAllows
#include "bdc.h"

/* Returns 1 when the innermost exception-specification frame (kind 2) of the exception-handling
   frame stack (`g_cxxEhFrameStack`, frames `{next, u8 kind, ...}`: 0 try block, 1 cleanup region,
   2 exception specification, 3 no-throw, 4 array construction) lists a type matching `type`/`flags`
   (`CxxEhMatchHandler`). */

int CxxEhSpecAllows(void *type, u8 flags, u32 extra)

{
  CxxEhSpecFrame *frame;
  u32 outEntry;

  for (frame = (CxxEhSpecFrame *)g_cxxEhFrameStack; frame != NULL && frame->kind != 2;
       frame = frame->next) {
  }
  if (frame->list != NULL &&
      CxxEhMatchHandler(frame->list, (int)(intptr_t)type, flags, extra, 0, 0, NULL, &outEntry) != 0) {
    return 1;
  }
  return 0;
}
