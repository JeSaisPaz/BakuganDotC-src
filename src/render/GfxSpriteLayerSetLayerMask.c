// bdc 0x089f5444 GfxSpriteLayerSetLayerMask
#include "bdc.h"

/* Sets the layer's draw mask (`+0x28`); only sprites whose `layerMask` shares a bit with it are
      drawn. */
void GfxSpriteLayerSetLayerMask(GfxSpriteLayer *self, u32 mask)
{
    self->layerMask = mask;
}
