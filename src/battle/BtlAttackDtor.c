// bdc 0x08876d34 BtlAttackDtor
#include "bdc.h"

/* Destructor of the battle attack object: restores the attack vtable,
   releases its two attached effects from their effect managers
   (UiSpriteLayerRelease), runs the CoreObject destructor and frees the object
   under the heap lock when flags bit 0 is set. NULL is ignored. */
void BtlAttackDtor(BtlAttack *self, u32 flags)
{
    GfxEffect *effect;

    if (self == NULL) {
        return;
    }
    effect = (GfxEffect *)self->effect;
    self->base.vtable = g_btlAttackVtbl;
    if (effect != NULL) {
        UiSpriteLayerRelease(effect->mgr, effect);
    }
    effect = (GfxEffect *)self->effect2;
    if (effect != NULL) {
        UiSpriteLayerRelease(effect->mgr, effect);
    }
    CoreObjectDtor(&self->base, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
