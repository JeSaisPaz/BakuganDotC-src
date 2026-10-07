// bdc 0x088ff9f4 BtlDemoDrawEffect
#include "bdc.h"

/* Draws the 3D sprite layer `layer` into render packet `packet` with the active camera moved to the
   origin: `BtlDemoCameraToOrigin`, `GfxSpriteLayerDrawWorld``(layer, packet, g_gfxActiveCamera,
   savedEye)`, `BtlDemoCameraRestore`. Does nothing when `layer` is NULL; `demo` is unused. */
void BtlDemoDrawEffect(void *demo, void *packet, GfxSpriteLayer *layer)
{
    float savedEye[4] __attribute__((aligned(16)));

    (void)demo;
    if (layer != NULL) {
        BtlDemoCameraToOrigin(savedEye);
        GfxSpriteLayerDrawWorld(layer, packet, g_gfxActiveCamera, savedEye);
        BtlDemoCameraRestore(savedEye);
    }
}
