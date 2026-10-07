// bdc 0x08a03d38 CxxRethrow
#include "bdc.h"

/* `throw;`: finds the innermost exception currently being handled on the current-exception stack
   (calls `CxxTerminateInternal` when there is none), pushes a rethrow record referring to it
   (`CxxEhPushException`) and throws (`CxxThrow`). */

void CxxRethrow(void)
{
  CxxEhRecord *orig;

  for (orig = (CxxEhRecord *)g_cxxEhCurrentException;
       orig != NULL && (orig->active == 0 || orig->isRethrow != 0);
       orig = orig->next) {
  }
  if (orig == NULL) {
    CxxTerminateInternal();
  }
  CxxEhPushException(orig->type, (void *)orig->dtor, orig->flags, orig->extra,
                     orig->extra2, orig->flags2, orig->object, 1, orig);
  CxxThrow();
}
