// bdc 0x088a6340 ActorStageObjMineState01Triggered
#include "bdc.h"

/* State 1 of the mine: every frame ramps the emissive colour towards red (+0.1/-0.1/-0.1) and the
   tint towards red (+0.05/-0.05/-0.05), copies the tint to the model colour and applies the
   emissive colour (`GfxModelSetAmbientColor`). Then runs the step sequence: steps 0/1 jump to 10,
   which spawns the explosion effect (0x57 + type for types 0..1, 0x143 for type 2,
   `GfxEffectSpawn`); 11 arms a 3-frame wait (12); 13/20 fade the alpha out by 0.07 per frame and
   once it is <= 0 stop the mine's own effects (`GfxEffectStopOwned`) and park at step 999. Any
   other step calls virtual slot 11 (`+0x58`). */

void ActorStageObjMineState01Triggered(ActorStageObjMine *self)
{
    const VtblEntry *entry;
    float fade;

    self->base.emissive[0] = self->base.emissive[0] + 0.1f;
    self->base.emissive[1] = self->base.emissive[1] - 0.1f;
    self->base.emissive[2] = self->base.emissive[2] - 0.1f;
    self->base.tint[0] = self->base.tint[0] + 0.05f;
    self->base.tint[1] = self->base.tint[1] - 0.05f;
    self->base.tint[2] = self->base.tint[2] - 0.05f;
    self->base.base.color[0] = self->base.tint[0];
    self->base.base.color[1] = self->base.tint[1];
    self->base.base.color[2] = self->base.tint[2];
    self->base.base.color[3] = self->base.tint[3];
    GfxModelSetAmbientColor(&self->base.base, self->base.emissive, NULL);

    switch ((u32)self->base.step) {
    case 0:
        self->base.step = self->base.step + 1;
        /* fallthrough */
    case 1:
        self->base.step = 10;
        /* fallthrough */
    case 10:
        if (self->mineType < 2) {
            if (self->mineType >= 0) {
                GfxEffectSpawn(g_btlUnitEffectMgr, self->mineType + 0x57, self->base.base.pos);
            }
        } else if (self->mineType < 3) {
            GfxEffectSpawn(g_btlUnitEffectMgr, 0x143, self->base.base.pos);
        }
        self->base.step = self->base.step + 1;
        /* fallthrough */
    case 11:
        self->fuseTimer = 3;
        self->base.step = self->base.step + 1;
        /* fallthrough */
    case 12:
        self->fuseTimer = self->fuseTimer - 1;
        if (self->fuseTimer > 0) {
            return;
        }
        self->base.step = self->base.step + 1;
        /* fallthrough */
    case 13:
        self->base.step = 20;
        /* fallthrough */
    case 20:
        fade = self->base.fade - 0.07f;
        self->base.fade = fade;
        if (fade <= 0.0f) {
            GfxEffectStopOwned(g_btlUnitEffectMgr, -1, self);
            self->base.step = 999;
        }
        return;
    default:
        entry = (const VtblEntry *)self->base.base.base.vtable + 11;
        ((void (*)(void *))entry->fn)((char *)self + entry->delta);
        return;
    }
}
