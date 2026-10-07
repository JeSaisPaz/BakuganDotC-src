// bdc 0x089f8ae8 GfxDeferredDelete
#include "bdc.h"

/* Queues a `CoreObject` for deletion at the next `GfxDeferredDeleteFlush`: unless it is already
   in the deferred list (`0x08b02c70`, `CoreObjectListContains`) it is unlinked from its current
   list (`CoreObjectUnlink`) and appended (`CoreObjectListAppend`). */

void GfxDeferredDelete(CoreObject *obj)

{
  s32 listed;
  
  listed = CoreObjectListContains(&g_gfxDeferredDeleteHead,obj);
  if (listed == 0) {
    CoreObjectUnlink(obj);
    CoreObjectListAppend(obj,(CoreObjectList *)&g_gfxDeferredDeleteHead);
  }
  return;
}

