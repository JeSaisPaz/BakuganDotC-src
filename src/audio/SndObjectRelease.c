// bdc 0x089c27e4 SndObjectRelease
#include "bdc.h"

/* Releases a sound object: if `obj` is non-NULL it calls `SndObjectDetachEmitters`, which clears `alive` (`+0x24`) and nulls the position source of every emitter. The object itself stays in the list until the next update destroys it; `owner` is not used. */

void SndObjectRelease(CoreNodeOwner *owner, CoreNode *obj)

{
  if (obj != (CoreNode *)0x0) {
    SndObjectDetachEmitters((SndObject *)obj);
  }
  return;
}

