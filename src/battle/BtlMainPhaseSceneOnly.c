// bdc 0x0884fbbc BtlMainPhaseSceneOnly
#include "bdc.h"

/* Phase 6 of the battle main task. With a battle outcome set (`g_btlBattleOutcome` non-zero) it
   switches `phase` and `drawPhase` to 2 and returns. Otherwise it fades the dim alpha
   (`dimColor[3]` × 0.9, snapped to 0 once at or below 0.005) and updates only the scene, without
   the unit logic: stop walls, the three model chains' motions, stage objects, the battle camera and
   the active camera's shake, the battle-end check (`BtlMainUpdateBattleEnd`), the active
   camera's projection (flags 2), HP gauges, attacks, items, stage ambient effects, the stage and
   unit effect managers (when present), sprite layers 2 and 1 and the mesh objects. */

void BtlMainPhaseSceneOnly(BtlMain *self)
{
    float dim;

    if (g_btlBattleOutcome != 0) {
        self->phase = 2;
        self->drawPhase = 2;
        return;
    }
    dim = self->dimColor[3];
    if (dim <= 0.00499999989f) {
        dim = 0.0f;
    } else {
        dim = dim * 0.899999976f;
    }
    self->dimColor[3] = dim;
    StopWallUpdateAll();
    GfxModelChainUpdateMotion(self->modelLists[0].head);
    GfxModelChainUpdateMotion(self->modelLists[1].head);
    GfxModelChainUpdateMotion(self->modelLists[2].head);
    ActorStageObjUpdateAll();
    BtlCameraUpdate(&self->camera);
    GfxCameraUpdateShake(g_gfxActiveCamera);
    BtlMainUpdateBattleEnd(self);
    GfxCameraUpdate(g_gfxActiveCamera, 2);
    UiHpGaugeUpdateAll();
    BtlAttackListUpdate();
    BtlItemListUpdateAll();
    BtlStageUpdateAmbientEffects();
    if (self->stageEffects != NULL) {
        GfxEffectMgrUpdate(self->stageEffects);
    }
    if (self->unitEffects != NULL) {
        GfxEffectMgrUpdate(self->unitEffects);
    }
    GfxSpriteLayerUpdateAll(self->spriteLayers[2]);
    GfxSpriteLayerUpdateAll(self->spriteLayers[1]);
    GfxMeshObjUpdateAll();
}
