// bdc 0x089f934c IoDiscCreate
#include "bdc.h"

/* Creates the disc-access manager singleton `g_discSimple` (`CODiscSimple`, 0x108 bytes,
   `IoDiscSimpleCtor`) if it does not exist, allocating from the low end of the heap, and
   returns `g_discSimple` (NULL if the allocation failed). */

IoDiscSimple *IoDiscCreate(void)
{
  IoDiscSimple *self;
  IoDiscSimple *obj;
  bool fromLow;

  if (g_discSimple == NULL) {
    obj = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    self = MemAlloc(sizeof(IoDiscSimple), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (self != NULL) {
      IoDiscSimpleCtor(self);
      obj = self;
    }
    g_discSimple = obj;
  }
  return g_discSimple;
}
