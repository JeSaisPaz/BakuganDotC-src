// bdc 0x0882361c GfxEffectUpdate
#include "bdc.h"

/* Per-frame update of an effect (vtable `0x08af16cc` slot 2). Skips effects already updated this
   tick (`lastTick` vs `g_gfxEffectTick`), counts the update in `g_gfxEffectCaptureCounter`,
   copies the followed matrix (`attachMatrix`, 16 floats) into the effect's own
   `matrix`, then increments `frame` and runs the command block for the current `key`
   (`GfxEffectFindKey`) plus the `persistentBlock` through `GfxEffectRunCommands`, repeating
   while the key block asks for another pass (> 0). The effect is released from its manager
   (`UiSpriteLayerRelease(mgr, effect)`, then return) when a block returns a negative value or
   no block is left; otherwise it ends with `GfxEffectUpdateWorldPos`. */

void GfxEffectUpdate(GfxEffect *effect)
{
    u8 *cmds;
    s32 pass;
    s32 i;

    if (effect->lastTick == g_gfxEffectTick) {
        return;
    }
    g_gfxEffectCaptureCounter = g_gfxEffectCaptureCounter + 1;
    pass = 1;
    if (effect->attachMatrix != NULL) {
        for (i = 0; i < 16; i++) {
            effect->matrix[i] = effect->attachMatrix[i];
        }
    }
    do {
        effect->frame = effect->frame + 1;
        cmds = GfxEffectFindKey(effect, effect->key);
        if (cmds != NULL) {
            pass = GfxEffectRunCommands(effect, cmds, effect->persistentBlock == NULL, pass);
            if (pass < 0) {
                UiSpriteLayerRelease(effect->mgr, effect);
                return;
            }
            if (effect->persistentBlock != NULL &&
                GfxEffectRunCommands(effect, effect->persistentBlock, 1, 1) < 0) {
                UiSpriteLayerRelease(effect->mgr, effect);
                return;
            }
        } else {
            if (effect->persistentBlock == NULL) {
                UiSpriteLayerRelease(effect->mgr, effect);
                return;
            }
            pass = 0;
            if (GfxEffectRunCommands(effect, effect->persistentBlock, 1, 1) < 0) {
                UiSpriteLayerRelease(effect->mgr, effect);
                return;
            }
        }
    } while (pass > 0);
    GfxEffectUpdateWorldPos(effect);
}
