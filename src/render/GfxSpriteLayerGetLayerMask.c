// bdc 0x089f544c GfxSpriteLayerGetLayerMask
#include "bdc.h"

/* Returns the layer's draw mask (`+0x28`). */
u32 GfxSpriteLayerGetLayerMask(GfxSpriteLayer *self)
{
    return self->layerMask;
}
