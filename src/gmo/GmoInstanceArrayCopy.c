// bdc 0x08a1cf0c GmoInstanceArrayCopy
#include "bdc.h"

/* Copies `count` `GmoInstance` records: returns NULL if `arr` or `plan` is NULL; with
   `flags & 0x81` carves a new array (`GmoPlanTakeRec20`), copies each record into it
   (`GmoInstanceCopy`) and returns the new array; otherwise bumps each record's `refCount` and
   returns `arr`. */

void *GmoInstanceArrayCopy(void *arr, s32 count, u32 flags, void *plan)
{
  GmoInstance *src = (GmoInstance *)arr;
  GmoInstance *copy;
  s32 i;

  if (arr == NULL || plan == NULL) {
    return NULL;
  }
  if ((flags & 0x81) != 0) {
    copy = (GmoInstance *)GmoPlanTakeRec20(count, plan);
    for (i = 0; i < count; i++) {
      GmoInstanceCopy(&copy[i], &src[i], flags, plan);
    }
    return copy;
  }
  for (i = 0; i < count; i++) {
    src[i].refCount++;
  }
  return arr;
}
