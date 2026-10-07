// bdc 0x08876ea0 BtlAttackEnd
#include "bdc.h"

/* Ends an attack: records the current age as its end frame (after which `BtlAttackUpdate`
   deletes it 0x50 frames later) and releases its two attached effect objects from their
   managers (`UiSpriteLayerRelease`), clearing both pointers. */

void BtlAttackEnd(BtlAttack *self)
{
    GfxEffect *effect;

    effect = (GfxEffect *)self->effect;
    self->endFrame = self->age;
    if (effect != NULL) {
        UiSpriteLayerRelease(effect->mgr, effect);
    }
    effect = (GfxEffect *)self->effect2;
    self->effect = NULL;
    if (effect != NULL) {
        UiSpriteLayerRelease(effect->mgr, effect);
    }
    self->effect2 = NULL;
}
