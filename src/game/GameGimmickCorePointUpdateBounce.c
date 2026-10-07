// bdc 0x0889fe4c GameGimmickCorePointUpdateBounce
#include "bdc.h"

/* Steps the bouncing flight of a launched core point (see `GameGimmickCorePointLaunch`):
   evaluates the current hop as a `CoreBezierCtor` curve `hop` (start `hopStart`, apex `hopMid`,
   end `hopEnd`, weight `hopWeight`) at `t = hopFrame / hopFrames` and copies the result to the model
   position. During the first hop (`hopCount == 0`) it scales a stack copy of `scale` by the eased
   `t` (weight `g_corePointBounceScaleWeight`; the copy is never used) and sets the model scale to
   (1, 1, 1). When `hopFrame` reaches `hopFrames`, `hopCount` is incremented; while it stays below
   `g_corePointHopCount` the next, shorter hop starts (step `hopStep` scaled by
   `g_corePointHopDecay` while `hopCount < count - 2`, `hopFrames = ``g_corePointHopFrames`` /
   hopCount`, weight 0.5, start = old end, end += step, height *= decay, apex = midpoint of start and
   end raised by `hopHeight`). After the last hop it stops the trail effect 0xe (if
   `effectAttached`), re-syncs the pickup shape `shapePos` to `pos` (shape virtual slot 9) and pins
   `hopFrame = hopFrames`, `hopCount = count`. */

void GameGimmickCorePointUpdateBounce(GameGimmickCorePoint *obj)

{
  float tmp0[4];
  float res[4];
  float scaled[4];
  float t;
  float s;

  t = (float)obj->hopFrame / (float)obj->hopFrames;
  /* stack temporary loaded with the bank's C720 (0, 0, 0, 0) */
  tmp0[0] = 0.0f;
  tmp0[1] = 0.0f;
  tmp0[2] = 0.0f;
  tmp0[3] = 0.0f;
  CoreBezierSet(obj->hopWeight, &obj->hop, &obj->hopStart.x, &obj->hopMid.x, &obj->hopEnd.x);
  CoreBezierEval(t, &obj->hop);
  s = g_corePointBounceScaleWeight * 2.0f * t * (1.0f - t) + t * t;
  if (obj->hopCount == 0) {
    /* dead stack copy of scale: xyz * s, w = bank S713 (0) */
    scaled[0] = obj->scale.x * s;
    scaled[1] = obj->scale.y * s;
    scaled[2] = obj->scale.z * s;
    scaled[3] = 0.0f;
    (void)scaled;
    obj->base.base.scale[0] = 1.0f;
    obj->base.base.scale[1] = 1.0f;
    obj->base.base.scale[2] = 1.0f;
  }
  res[0] = obj->hop.result.x;
  res[1] = obj->hop.result.y;
  res[2] = obj->hop.result.z;
  res[3] = obj->hop.result.w;
  tmp0[0] = res[0];
  tmp0[1] = res[1];
  tmp0[2] = res[2];
  (void)tmp0;
  obj->base.base.pos[0] = res[0];
  obj->base.base.pos[1] = res[1];
  obj->base.base.pos[2] = res[2];

  obj->hopFrame = obj->hopFrame + 1;
  if (obj->hopFrame < obj->hopFrames) {
    return;
  }
  obj->hopCount = obj->hopCount + 1;
  if (!(obj->hopCount < g_corePointHopCount)) {
    if (obj->effectAttached) {
      GfxEffectStopAttached(g_worldEffectMgr, 0xe, &obj->base.base.data->rootMatrix[12]);
      obj->shapePos.x = obj->base.base.pos[0];
      obj->shapePos.y = obj->base.base.pos[1];
      obj->shapePos.z = obj->base.base.pos[2];
      obj->shapePos.w = obj->base.base.pos[3];
      ((void (*)(void *))obj->shapeVtbl[9].fn)((char *)&obj->shapeType + obj->shapeVtbl[9].delta);
      obj->effectAttached = 0;
    }
    obj->hopFrame = obj->hopFrames;
    obj->hopCount = g_corePointHopCount;
    return;
  }

  if ((s32)obj->hopCount < (s32)g_corePointHopCount - 2) {
    /* xyz * decay; `vscl.t` + `sv.q` store the bank's S713 (0) into w */
    obj->hopStep.x = obj->hopStep.x * g_corePointHopDecay;
    obj->hopStep.y = obj->hopStep.y * g_corePointHopDecay;
    obj->hopStep.z = obj->hopStep.z * g_corePointHopDecay;
    obj->hopStep.w = 0.0f;
  }
  obj->hopFrame = 0;
  obj->hopFrames = (s16)((s32)g_corePointHopFrames / (s32)obj->hopCount);
  obj->hopWeight = 0.5f;
  obj->hopStart.x = obj->hopEnd.x;
  obj->hopStart.y = obj->hopEnd.y;
  obj->hopStart.z = obj->hopEnd.z;
  /* hopEnd.xyz += hopStep.xyz (w kept) */
  obj->hopEnd.x = obj->hopEnd.x + obj->hopStep.x;
  obj->hopEnd.y = obj->hopEnd.y + obj->hopStep.y;
  obj->hopEnd.z = obj->hopEnd.z + obj->hopStep.z;
  obj->hopHeight = obj->hopHeight * g_corePointHopDecay;
  /* hopMid.xyz = (hopEnd + hopStart) * 0.5; w = bank S713 (0) */
  obj->hopMid.x = (obj->hopEnd.x + obj->hopStart.x) * 0.5f;
  obj->hopMid.y = (obj->hopEnd.y + obj->hopStart.y) * 0.5f;
  obj->hopMid.z = (obj->hopEnd.z + obj->hopStart.z) * 0.5f;
  obj->hopMid.w = 0.0f;
  if (g_staticZeroFGuard == 0) {
    g_staticZeroFGuard = 1;
    g_staticZeroF = 0.0f;
  }
  obj->hopMid.y = obj->hopMid.y + obj->hopHeight;
}
