// bdc 0x08a14158 GmoRec20Ctor
#include "bdc.h"

/* In-place constructor of the 0x20-byte records carved by `GmoPlanTakeRec20`: sets the reference
   count `+0x0` to 1 and clears the rest. Returns `rec` (NULL-safe). */

void *GmoRec20Ctor(void *rec)
{
  GmoInstance *inst = (GmoInstance *)rec;

  if (inst != (GmoInstance *)0x0) {
    inst->refCount = 1;
    inst->pad02 = 0;
    inst->next = (GmoInstance *)0x0;
    inst->displayList = (unsigned int *)0x0;
    inst->displayListWords = 0;
    inst->id = 0;
    inst->state = (void *)0x0;
    inst->vertices = (void *)0x0;
    inst->extra = 0;
    inst->vertexSize = 0;
    inst->vertexCount = 0;
  }
  return rec;
}
