// bdc 0x08a1480c GmoInstanceArrayRelease
#include "bdc.h"

/* Drops one reference on each of `n` 0x20-byte GMO mesh-instance records; an instance reaching 0
   releases its child array (`+4`, recursively), its display list (`+8`), state block (`+0x10`) and
   vertex data (`+0x14`) from pool 1, and itself. Returns `arr`. */

short *GmoInstanceArrayRelease(short *arr, int n)

{
  GmoInstance *inst;
  int i;

  if (arr != NULL) {
    inst = (GmoInstance *)arr;
    for (i = 0; i < n; i++, inst++) {
      inst->refCount = inst->refCount - 1;
      if (inst->refCount == 0) {
        GmoInstanceArrayRelease((short *)inst->next, 1);
        GmoHeapReleaseThunk(1, inst->displayList);
        GmoHeapReleaseThunk(1, inst->vertices);
        GmoHeapReleaseThunk(1, inst->state);
        GmoHeapReleaseThunk(0, inst);
      }
    }
  }
  return arr;
}
