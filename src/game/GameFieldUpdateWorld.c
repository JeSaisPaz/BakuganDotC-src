// bdc 0x088be91c GameFieldUpdateWorld
#include "bdc.h"

/* Per-frame world update of the field task (id 500, `GameFieldCtor`) (called by
   `GameFieldPhaseMain`): counts frames (`frameCount`), updates the stage records, field points and
   stage objects (`ActorStageObjRecordUpdateAll`, `GameFieldPointListUpdate`,
   `ActorStageObjUpdateAll`), the model lists `npcs`, `objList634`, `objList640`, `ballList`,
   `gimmicks`, `objList670` (`GfxModelChainUpdateMotion`), the route groups
   (`ActorSyncRouteGroups`), the camera (`GameFieldCameraUpdate`, `GfxCameraUpdateShake`,
   `GfxCameraUpdate` of `g_gfxActiveCamera`); when the world-map task `worldMapTask` exists it spins
   its model (`+0x520`) root matrix about Y by 0.01 rad per frame and advances the wrapped scroll values of that
   model and of `g_gameStageExtraModels`[0]; runs `GameFieldUpdateBackgroundBattle` on stage 1
   (script variable 1), then the effect managers `effectMgr`/`effectMgr2` (`GfxEffectMgrUpdate`), the
   screen effects `screenFx` (`GameFieldScreenFxUpdate`), the sprite layers `layers[2]`, `[0]`, `[1]`
   (`GfxSpriteLayerUpdateAll`) and all mesh objects (`GfxMeshObjUpdateAll`). */

/* One field of `rot * m` (VFPU `vmmul.q E200, E100, E000`): lane by lane, summed left to right,
   with rot's columns (c, 0, -s, 0), (0, 1, 0, 0), (s, 0, c, 0), (0, 0, 0, 1). */
static void GameFieldRotateFieldY(float *d, const float *r, float c, float s)
{
  float x = r[0];
  float y = r[1];
  float z = r[2];
  float w = r[3];

  d[0] = x * c + y * 0.0f + z * s + w * 0.0f;
  d[1] = x * 0.0f + y * 1.0f + z * 0.0f + w * 0.0f;
  d[2] = x * -s + y * 0.0f + z * c + w * 0.0f;
  d[3] = x * 0.0f + y * 0.0f + z * 0.0f + w * 1.0f;
}

void GameFieldUpdateWorld(CoreTask *task)
{
  GameFieldTask *field = (GameFieldTask *)task;

  field->frameCount = field->frameCount + 1;
  ActorStageObjRecordUpdateAll();
  GameFieldPointListUpdate();
  ActorStageObjUpdateAll();
  GfxModelChainUpdateMotion((CoreObject *)field->npcs);
  GfxModelChainUpdateMotion(field->objList634.head);
  GfxModelChainUpdateMotion(field->objList640.head);
  GfxModelChainUpdateMotion(((CoreObjectList *)field->ballList)->head);
  GfxModelChainUpdateMotion((CoreObject *)field->gimmicks);
  GfxModelChainUpdateMotion(field->objList670.head);
  ActorSyncRouteGroups();
  GameFieldCameraUpdate((GameFieldCamera *)field->camera);
  GfxCameraUpdateShake(g_gfxActiveCamera);
  GfxCameraUpdate(g_gfxActiveCamera, 2);
  if (field->worldMapTask != NULL) {
    GfxModel *model = ((GfxPlayerBlurTask *)field->worldMapTask)->model;
    GfxModel *extra;
    float *m = model->data->rootMatrix;
    /* vmul.s by S703 (bank 2/pi) then vrot in quarter turns: cos/sin of 0.01 rad */
    float c = __builtin_cosf(0.01f);
    float s = __builtin_sinf(0.01f);
    int i;

    for (i = 0; i < 4; i++) {
      float r[4];

      r[0] = m[i * 4 + 0];
      r[1] = m[i * 4 + 1];
      r[2] = m[i * 4 + 2];
      r[3] = m[i * 4 + 3];
      GameFieldRotateFieldY(&m[i * 4], r, c, s);
    }

    model->velocity[0] = model->velocity[0] + 0.00333333341f;
    if (!(model->velocity[0] < 1.0f)) {
      model->velocity[0] = model->velocity[0] - 1.0f;
    }
    extra = (GfxModel *)g_gameStageExtraModels[0];
    extra->velocity[0] = extra->velocity[0] + 0.01f;
    if (!(extra->velocity[0] < 1.0f)) {
      extra->velocity[0] = extra->velocity[0] - 1.0f;
    }
    extra->velocity[1] = extra->velocity[1] + 0.00625f;
    if (!(extra->velocity[1] < 1.0f)) {
      extra->velocity[1] = extra->velocity[1] - 1.0f;
    }
    if (extra->velocity[2] == 0.0f) {
      extra->velocity[2] = 1.0f;
    } else {
      extra->velocity[2] = 0.0f;
    }
  }
  if (g_scriptGlobalVars[1] == 1) {
    GameFieldUpdateBackgroundBattle(task);
  }
  if (field->effectMgr != NULL) {
    GfxEffectMgrUpdate(field->effectMgr);
  }
  if (field->effectMgr2 != NULL) {
    GfxEffectMgrUpdate(field->effectMgr2);
  }
  if (field->screenFx != NULL) {
    GameFieldScreenFxUpdate(field->screenFx);
  }
  GfxSpriteLayerUpdateAll(field->layers[2]);
  GfxSpriteLayerUpdateAll(field->layers[0]);
  GfxSpriteLayerUpdateAll(field->layers[1]);
  GfxMeshObjUpdateAll();
}
