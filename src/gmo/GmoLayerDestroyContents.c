// bdc 0x08a14250 GmoLayerDestroyContents
#include "bdc.h"

/* Releases the texture referenced by a 0x10-byte GMO texture-layer record (`texture`,
   `GmoTextureRelease`) without freeing the record. Returns the record. */

GmoLayer *GmoLayerDestroyContents(GmoLayer *layer)

{
  if (layer != (GmoLayer *)0x0) {
    GmoTextureRelease((short *)layer->texture);
  }
  return layer;
}
