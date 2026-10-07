// bdc 0x0884bc80 BtlMainUpdateScene
#include "bdc.h"

/* Per-frame scene update of the battle main task: increments g_btlSceneFrameCount every call;
   while phaseStep < 100 it updates the stage-object records, the motions of the three model
   lists, the stage objects, the follow camera (BtlCameraUpdate, active-camera shake,
   BtlMainUpdateBattleEnd, GfxCameraUpdate with flags 2), stage UV animations, HP gauges, attacks,
   items, stage ambient effects, the stage and unit effect sets (when present), sprite layers 2
   and 1 and the mesh objects. Then focusBlend moves 0.05 toward 1 (clamped, also for NaN) while
   focusFrames > 0, decrementing focusFrames, or 0.05 toward 0 (clamped) otherwise, and focus
   xyzw eases toward focusTarget by e = (1 - cos(pi * focusBlend)) / 2 (focus += (focusTarget -
   focus) * e); afterwards focus[3] is overwritten with e * focusTarget[3]. The cosine is the VFPU
   quarter-turn vcos of the angle times S703 (2/pi), i.e. cos(pi * focusBlend). */

void BtlMainUpdateScene(BtlMain *self)
{
    float blend;
    float cosine;
    float ease;
    int i;

    g_btlSceneFrameCount = g_btlSceneFrameCount + 1;
    if (self->phaseStep < 100) {
        ActorStageObjRecordUpdateAll();
        GfxModelChainUpdateMotion(self->modelLists[0].head);
        GfxModelChainUpdateMotion(self->modelLists[1].head);
        GfxModelChainUpdateMotion(self->modelLists[2].head);
        ActorStageObjUpdateAll();
        BtlCameraUpdate(&self->camera);
        GfxCameraUpdateShake(g_gfxActiveCamera);
        BtlMainUpdateBattleEnd(self);
        GfxCameraUpdate(g_gfxActiveCamera, 2);
        BtlStageUpdateUvAnims();
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
        if (self->focusFrames < 1) {
            blend = self->focusBlend - 0.0500000007f;
            if (blend < 0.0f) {
                blend = 0.0f;
            }
            self->focusBlend = blend;
        } else {
            blend = self->focusBlend + 0.0500000007f;
            if (!(blend <= 1.0f)) {
                blend = 1.0f;
            }
            self->focusBlend = blend;
            self->focusFrames = self->focusFrames - 1;
        }

        cosine = __builtin_cosf(self->focusBlend * 3.14159274f);
        ease = (1.0f - cosine) * 0.5f;
        /* focus += (focusTarget - focus) * ease, all four lanes */
        for (i = 0; i < 4; i++) {
            self->focus[i] = self->focus[i] + (self->focusTarget[i] - self->focus[i]) * ease;
        }
        cosine = __builtin_cosf(self->focusBlend * 3.14159274f);
        self->focus[3] = (1.0f - cosine) * 0.5f * self->focusTarget[3];
    }
}
