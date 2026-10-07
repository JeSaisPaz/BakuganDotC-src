// bdc 0x0889abe8 BtlAnyUnitHasEntrySequence
#include "bdc.h"

/* Returns 1 when some unit in the battle unit list (`BtlGetBakuganList`) whose vtable entry 13
   (`BtlBakuganIsCpuUnit` in the base class) returns nonzero has a ball entry pending
   (`ballEntryPending` != 0); 0 otherwise or when the list is missing. */
s32 BtlAnyUnitHasEntrySequence(void)
{
  CoreObjectList *list = (CoreObjectList *)BtlGetBakuganList();
  CoreObject *obj;

  if (list == NULL) {
    return 0;
  }
  for (obj = list->head; obj != NULL; obj = obj->next) {
    const VtblEntry *isCpuUnit = &((const VtblEntry *)obj->vtable)[13];

    if (((int (*)(void *))isCpuUnit->fn)((u8 *)obj + isCpuUnit->delta) != 0 &&
        ((BtlBakugan *)obj)->ballEntryPending != 0) {
      return 1;
    }
  }
  return 0;
}
