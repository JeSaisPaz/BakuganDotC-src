// bdc 0x088a7bf4 ActorStageObjAttrLandmarkSetEffectsVisible
#include "bdc.h"

/* Shows (`visible != 0`, alpha 1) or hides (alpha 0) all effects of an attribute landmark: on
   `g_worldEffectMgr` its owned effects (`GfxEffectSetAlphaOwned`) and the effects attached to the
   model's root-matrix translation and to the three `effectPos` points (`GfxEffectSetAlphaAttached`),
   then its owned effects on `g_btlUnitEffectMgr`; when shown, its effect ids 0xfe/0xff on
   `g_btlUnitEffectMgr` are then dimmed to 0.2. Called by `ActorStageObjSetAttrLandmarksVisible`. */

void ActorStageObjAttrLandmarkSetEffectsVisible(ActorStageObjAttrLandmark *self, char visible)
{
    if ((u8)visible != 0) {
        GfxEffectSetAlphaOwned(1.0f, g_worldEffectMgr, -1, self);
        GfxEffectSetAlphaAttached(1.0f, g_worldEffectMgr, -1, &self->base.base.data->rootMatrix[12]);
        GfxEffectSetAlphaAttached(1.0f, g_worldEffectMgr, -1, self->effectPos[0]);
        GfxEffectSetAlphaAttached(1.0f, g_worldEffectMgr, -1, self->effectPos[1]);
        GfxEffectSetAlphaAttached(1.0f, g_worldEffectMgr, -1, self->effectPos[2]);
        GfxEffectSetAlphaOwned(1.0f, g_btlUnitEffectMgr, -1, self);
        GfxEffectSetAlphaOwned(0.2f, g_btlUnitEffectMgr, 0xfe, self);
        GfxEffectSetAlphaOwned(0.2f, g_btlUnitEffectMgr, 0xff, self);
    } else {
        GfxEffectSetAlphaOwned(0.0f, g_worldEffectMgr, -1, self);
        GfxEffectSetAlphaAttached(0.0f, g_worldEffectMgr, -1, &self->base.base.data->rootMatrix[12]);
        GfxEffectSetAlphaAttached(0.0f, g_worldEffectMgr, -1, self->effectPos[0]);
        GfxEffectSetAlphaAttached(0.0f, g_worldEffectMgr, -1, self->effectPos[1]);
        GfxEffectSetAlphaAttached(0.0f, g_worldEffectMgr, -1, self->effectPos[2]);
        GfxEffectSetAlphaOwned(0.0f, g_btlUnitEffectMgr, -1, self);
    }
}
