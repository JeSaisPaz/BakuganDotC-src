// bdc 0x08a148c0 GmoInstanceDestroyContents
#include "bdc.h"

/* Releases what one 0x20-byte GMO mesh-instance record references (child array `+4` via
   `GmoInstanceArrayRelease`, pool-1 blocks `+8`, `+0x14`, `+0x10`) without freeing the record.
   Returns the record. */

void *GmoInstanceDestroyContents(void *inst)

{
  GmoInstance *p = (GmoInstance *)inst;

  if (p != NULL) {
    GmoInstanceArrayRelease((short *)p->next, 1);
    GmoHeapReleaseThunk(1, p->displayList);
    GmoHeapReleaseThunk(1, p->vertices);
    GmoHeapReleaseThunk(1, p->state);
  }
  return inst;
}
