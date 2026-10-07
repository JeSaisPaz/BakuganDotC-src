// bdc 0x088a0604 GameGimmickCorePointLaunch
#include "bdc.h"

/* Launches a core point out of a spawner, once (`launched` is set on the first call; later calls
   do nothing). The hop starts at `startPos` (w from the model position) and its landing point is
   13 units along `dir` in x/z (dir.y unscaled), rotated by -45 degrees for odd `index` and +45 for
   even `index` > 0 (index <= 0: no rotation). `hopStep` is 2 * dir (w 0), rotated the same way. The
   start is lifted 14 units. The mid point is the halfway point raised by half the start/end
   height difference. A 500-unit segment from the mid point towards the landing point
   (`g_collisionSegmentDesc` in `g_collisionSegmentBlock2`, `CollisionRaycast` layer mask
   0x1bf077e) snaps the landing point to the hit, but never below `startPos.y`. The mid point is
   then rebuilt with the apex factor `g_corePointLaunchApexScale` and copied to `hopHeight`.
   `hopFrames` is `g_corePointLaunchFrames` times the planned/actual travel length ratio
   (lengths clamped to >= 1e-5, ratio to >= 1). `hopWeight` comes from `g_corePointLaunchWeight`.
   The function spawns the trail effect 0xe attached to the model's root matrix translation
   (`GfxEffectSpawnAttached` on `g_worldEffectMgr`; texture slot 4, size 2), sets
   `effectAttached`, moves the model to `hopStart` and saves the model scale in `scale`. The flight
   itself is run by `GameGimmickCorePointUpdateBounce`. */

/* Inlined lazy init of the static zero float (one copy per inlined vector constructor). */
#define STATIC_ZERO_GUARD()        \
  do {                             \
    if (g_staticZeroFGuard == 0) { \
      g_staticZeroFGuard = 1;      \
      g_staticZeroF = 0.0f;        \
    }                              \
  } while (0)

void GameGimmickCorePointLaunch(GameGimmickCorePoint *obj, int index, const float *dir, const float *startPos)
{
  GfxEffect *effect;
  float angle, c, s;
  float lenSq, k;
  float plannedLen, travelLen, ratio;
  float x, y, z;
  float m00, m01, m02, m10, m11, m12, m20, m21, m22;
  ScePspFVector4 start, planned, aim, rayDir, rayEnd, origin, end, hit;

  if (obj->launched != 0) {
    return;
  }
  /* The player is looked up for a normalised player-to-self direction whose x/y/z are overwritten
     by `dir` right away; only its w (0) survives, so the lookup's result is unused. */
  ActorFindPlayer();
  obj->launched = 1;
  start.x = startPos[0];
  start.y = startPos[1];
  start.z = startPos[2];
  start.w = obj->base.base.pos[3];
  planned.x = 0.0f;
  planned.y = 0.0f;
  planned.z = 0.0f;
  planned.w = 0.0f;
  aim.x = dir[0];
  aim.y = dir[1];
  aim.z = dir[2];
  aim.w = 0.0f;
  obj->hopStart = start;
  obj->hopEnd = start;
  obj->hopStep = aim;
  STATIC_ZERO_GUARD();
  obj->hopStart.y = obj->hopStart.y + 14.0f;
  aim.x = aim.x * 13.0f;
  aim.z = aim.z * 13.0f;
  obj->hopStep.x = obj->hopStep.x * 2.0f;
  obj->hopStep.y = obj->hopStep.y * 2.0f;
  obj->hopStep.z = obj->hopStep.z * 2.0f;
  obj->hopStep.w = 0.0f;
  if (index > 0) {
    angle = (index % 2 != 0) ? -0.78539819f : 0.78539819f;
    /* rotation about Y: rows (c, 0, -s), (0, 1, 0), (s, 0, c) */
    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    m00 = c;
    m01 = 0.0f;
    m02 = -s;
    m10 = 0.0f;
    m11 = 1.0f;
    m12 = 0.0f;
    m20 = s;
    m21 = 0.0f;
    m22 = c;
    x = aim.x;
    y = aim.y;
    z = aim.z;
    aim.x = m00 * x + m01 * y + m02 * z;
    aim.y = m10 * x + m11 * y + m12 * z;
    aim.z = m20 * x + m21 * y + m22 * z;
    x = obj->hopStep.x;
    y = obj->hopStep.y;
    z = obj->hopStep.z;
    obj->hopStep.x = m00 * x + m01 * y + m02 * z;
    obj->hopStep.y = m10 * x + m11 * y + m12 * z;
    obj->hopStep.z = m20 * x + m21 * y + m22 * z;
  }
  /* hopEnd += aim; planned = hopEnd - hopStart; hopMid = (hopEnd + hopStart) * 0.5 */
  obj->hopEnd.x = obj->hopEnd.x + aim.x;
  obj->hopEnd.y = obj->hopEnd.y + aim.y;
  obj->hopEnd.z = obj->hopEnd.z + aim.z;
  planned.x = obj->hopEnd.x - obj->hopStart.x;
  planned.y = obj->hopEnd.y - obj->hopStart.y;
  planned.z = obj->hopEnd.z - obj->hopStart.z;
  planned.w = obj->hopEnd.w;
  obj->hopMid.x = (obj->hopEnd.x + obj->hopStart.x) * 0.5f;
  obj->hopMid.y = (obj->hopEnd.y + obj->hopStart.y) * 0.5f;
  obj->hopMid.z = (obj->hopEnd.z + obj->hopStart.z) * 0.5f;
  obj->hopMid.w = 0.0f;
  STATIC_ZERO_GUARD();
  STATIC_ZERO_GUARD();
  STATIC_ZERO_GUARD();
  obj->hopMid.y = obj->hopMid.y + (obj->hopStart.y - obj->hopEnd.y) * 0.5f;
  /* rayEnd = hopMid + normalize(hopEnd - hopMid) * 500 (normalised lanes clamped to [-1, 1]) */
  rayDir.x = obj->hopEnd.x - obj->hopMid.x;
  rayDir.y = obj->hopEnd.y - obj->hopMid.y;
  rayDir.z = obj->hopEnd.z - obj->hopMid.z;
  lenSq = rayDir.x * rayDir.x + rayDir.y * rayDir.y + rayDir.z * rayDir.z;
  if (lenSq == 0.0f) {
    k = 0.0f;
  } else {
    k = VfRsq(lenSq);
  }
  rayDir.x = VfSat1(rayDir.x * k) * 500.0f;
  rayDir.y = VfSat1(rayDir.y * k) * 500.0f;
  rayDir.z = VfSat1(rayDir.z * k) * 500.0f;
  rayDir.w = 0.0f;
  rayEnd.x = obj->hopMid.x + rayDir.x;
  rayEnd.y = obj->hopMid.y + rayDir.y;
  rayEnd.z = obj->hopMid.z + rayDir.z;
  rayEnd.w = obj->hopMid.w;
  STATIC_ZERO_GUARD();
  STATIC_ZERO_GUARD();
  STATIC_ZERO_GUARD();
  origin.x = obj->hopMid.x;
  origin.y = obj->hopMid.y;
  origin.z = obj->hopMid.z;
  origin.w = 0.0f;
  STATIC_ZERO_GUARD();
  STATIC_ZERO_GUARD();
  STATIC_ZERO_GUARD();
  end.x = rayEnd.x;
  end.y = rayEnd.y;
  end.z = rayEnd.z;
  end.w = 0.0f;
  /* segment query: start = origin, dir = end - origin */
  g_collisionSegmentDesc.start[0] = origin.x;
  g_collisionSegmentDesc.start[1] = origin.y;
  g_collisionSegmentDesc.start[2] = origin.z;
  g_collisionSegmentDesc.start[3] = origin.w;
  g_collisionSegmentDesc.dir[0] = end.x - origin.x;
  g_collisionSegmentDesc.dir[1] = end.y - origin.y;
  g_collisionSegmentDesc.dir[2] = end.z - origin.z;
  g_collisionSegmentDesc.dir[3] = end.w;
  if (CollisionRaycast(0x1bf077e, &g_collisionSegmentBlock2, 0) != NULL) {
    hit = g_collisionHitResult.point;
    if (!(startPos[1] <= hit.y)) {
      hit.y = startPos[1];
    }
    obj->hopEnd = hit;
  }
  /* hopMid = (hopEnd + hopStart) * 0.5, then raised by the apex factor */
  obj->hopMid.x = (obj->hopEnd.x + obj->hopStart.x) * 0.5f;
  obj->hopMid.y = (obj->hopEnd.y + obj->hopStart.y) * 0.5f;
  obj->hopMid.z = (obj->hopEnd.z + obj->hopStart.z) * 0.5f;
  obj->hopMid.w = 0.0f;
  STATIC_ZERO_GUARD();
  STATIC_ZERO_GUARD();
  STATIC_ZERO_GUARD();
  obj->hopMid.y = obj->hopMid.y + (obj->hopStart.y - obj->hopEnd.y) * g_corePointLaunchApexScale;
  STATIC_ZERO_GUARD();
  obj->hopHeight = obj->hopMid.y;
  /* |planned| and |hopEnd - hopStart| */
  x = obj->hopEnd.x - obj->hopStart.x;
  y = obj->hopEnd.y - obj->hopStart.y;
  z = obj->hopEnd.z - obj->hopStart.z;
  plannedLen = __builtin_sqrtf(planned.x * planned.x + planned.y * planned.y + planned.z * planned.z);
  travelLen = __builtin_sqrtf(x * x + y * y + z * z);
  if (plannedLen <= 1e-05f) {
    plannedLen = 1e-05f;
  }
  if (travelLen <= 1e-05f) {
    travelLen = 1e-05f;
  }
  ratio = plannedLen / travelLen;
  if (ratio < 1.0f) {
    ratio = 1.0f;
  }
  obj->hopFrame = 0;
  obj->hopCount = 0;
  obj->hopFrames = (s16)(int)((float)g_corePointLaunchFrames * ratio);
  obj->hopWeight = g_corePointLaunchWeight;
  effect = (GfxEffect *)GfxEffectSpawnAttached(g_worldEffectMgr, 0xe, &obj->base.base.data->rootMatrix[12]);
  effect->textureSlot = 4;
  effect->vec1d0[1] = 4.0f;
  effect->size[0] = 2.0f;
  effect->size[1] = 2.0f;
  effect->size[2] = 2.0f;
  effect->size[3] = 0.0f;
  obj->effectAttached = 1;
  obj->base.base.pos[0] = obj->hopStart.x;
  obj->base.base.pos[1] = obj->hopStart.y;
  obj->base.base.pos[2] = obj->hopStart.z;
  obj->scale.x = obj->base.base.scale[0];
  obj->scale.y = obj->base.base.scale[1];
  obj->scale.z = obj->base.base.scale[2];
  obj->scale.w = obj->base.base.scale[3];
}
