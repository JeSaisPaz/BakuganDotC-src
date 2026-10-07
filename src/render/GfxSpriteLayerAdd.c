// bdc 0x089f5164 GfxSpriteLayerAdd
#include "bdc.h"

/* Appends a sprite (or any `CoreObject`-based drawable, e.g. an effect) to the layer's draw list
   at `+0x1c` (`CoreObjectListAppend`). Returns `obj`. */

CoreObject *GfxSpriteLayerAdd(GfxSpriteLayer *self, CoreObject *obj)
{
  return CoreObjectListAppend(obj, (CoreObjectList *)&self->head);
}
