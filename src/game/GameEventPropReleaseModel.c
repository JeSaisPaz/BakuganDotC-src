// bdc 0x088ea4ec GameEventPropReleaseModel
#include "bdc.h"

/* Unlinks the prop's model (`CoreObjectUnlink`), releases it (`CoreObjectDeferDelete(model, 0)`) and
   clears `+0`. */

void GameEventPropReleaseModel(void *prop)

{
  if (*(void **)prop != (void *)0x0) {
    CoreObjectUnlink(*(void **)prop);
    CoreObjectDeferDelete(*(CoreObject **)prop,0);
    *(void **)prop = (void *)0x0;
  }
  return;
}
