// bdc 0x0884cc2c BtlMainOnDemoEnd
#include "bdc.h"

/* Called by `BtlDemoFinish` when a battle demo ends: republishes the main task's sprite
   layers and effect managers into their globals (spriteLayers[1] into g_billboardSpriteLayer
   and g_puffSpriteLayer, spriteLayers[2] into g_worldSpriteLayer and g_worldSpriteLayerCopy,
   stageEffects into g_worldEffectMgr, unitEffects into g_btlUnitEffectMgr), rebinds the sound
   listener to the battle camera, snaps the camera back to its default follow
   (`BtlCameraSetDefaultFollow` with snap 1), runs one stage-object update
   (`ActorStageObjUpdateAll`), turns the field effects on (`BtlSetFieldEffectsActive`) and
   sets g_btlCameraDefaultMode to 1. */

void BtlMainOnDemoEnd(BtlMain *self)
{
    g_billboardSpriteLayer = self->spriteLayers[1];
    g_puffSpriteLayer = self->spriteLayers[1];
    g_worldSpriteLayer = self->spriteLayers[2];
    g_worldSpriteLayerCopy = self->spriteLayers[2];
    g_worldEffectMgr = self->stageEffects;
    g_btlUnitEffectMgr = self->unitEffects;
    BtlCameraBindListener(&self->camera);
    BtlCameraSetDefaultFollow(&self->camera, 1);
    ActorStageObjUpdateAll();
    BtlSetFieldEffectsActive(true);
    g_btlCameraDefaultMode = 1;
}
