// bdc 0x08a03af4 CxxEhReleaseException
#include "bdc.h"

/* Drops one use of an exception record (or of the original when it is a rethrow copy): when the use
   count reaches 0 and the object was constructed (`+0x31`), runs its destructor (`dtor(obj, 2)`)
   unless it is a pointer type. */

void CxxEhReleaseException(void *exc)

{
  CxxEhRecord *rec = (CxxEhRecord *)exc;
  CxxEhRecord *target = rec;

  if (rec->isRethrow != 0) {
    target = rec->orig;
  }
  if (rec->released == 0) {
    rec->released = 1;
    target->useCount = target->useCount - 1;
  }
  if ((target->useCount == 0) && (target->destroyed == 0)) {
    target->destroyed = 1;
    if ((target->constructed != 0) && ((target->flags & 1) == 0) && (target->dtor != NULL)) {
      target->dtor(target->object, 2);
    }
  }
}
