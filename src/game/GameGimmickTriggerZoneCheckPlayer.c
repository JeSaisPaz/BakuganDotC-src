// bdc 0x088da288 GameGimmickTriggerZoneCheckPlayer
#include "bdc.h"

/* While the zone is active (`active`), clears bit 2 of `contactFlags`, tests whether the player
   stands in it (`GameGimmickTriggerZoneContainsPlayer`) and sets the bit again when it does. For
   type id `0x324` it also spawns effect 0x41 once (`GfxEffectSpawn` on `g_worldEffectMgr`,
   handle `effect`) and plays sound `0x2c00043` on entering (`inside` was 0). The colour `color`
   eases 10% per frame toward white (inside) or zero (outside) (vector
   lerp, VFPU in the binary); for type `0x324` with an effect it is then pushed to effects
   0x41..0x43 (`GfxEffectSetColorAttached`). `inside` remembers the result for the next frame. */

void GameGimmickTriggerZoneCheckPlayer(GameGimmickTriggerZone *gimmick)
{
  ScePspFVector4 goal;
  float rate;

  if (gimmick->base.active == 0) {
    return;
  }
  gimmick->base.contactFlags &= ~2;
  /* the binary also passes a vector built from the record (+0xc ints / 4096, w = 0) in a1/a2;
     the callee never reads them */
  if (GameGimmickTriggerZoneContainsPlayer(gimmick) == 1) {
    gimmick->base.contactFlags |= 2;
    if (gimmick->effect == NULL && gimmick->base.typeId == 0x324) {
      gimmick->effect = GfxEffectSpawn(g_worldEffectMgr, 0x41, gimmick->base.base.pos);
    }
    if (gimmick->base.typeId == 0x324 && gimmick->inside == 0 && SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c00043, 0, 0);
    }
    gimmick->inside = 1;
    goal.x = 1.0f;
    goal.y = 1.0f;
    goal.z = 1.0f;
    goal.w = 1.0f;
    rate = 0.1f;
    /* color += (goal - color) * rate */
    gimmick->color.x = gimmick->color.x + (goal.x - gimmick->color.x) * rate;
    gimmick->color.y = gimmick->color.y + (goal.y - gimmick->color.y) * rate;
    gimmick->color.z = gimmick->color.z + (goal.z - gimmick->color.z) * rate;
    gimmick->color.w = gimmick->color.w + (goal.w - gimmick->color.w) * rate;
  } else {
    goal.x = 0.0f;
    goal.y = 0.0f;
    goal.z = 0.0f;
    goal.w = 0.0f;
    rate = 0.1f;
    /* color += (goal - color) * rate */
    gimmick->color.x = gimmick->color.x + (goal.x - gimmick->color.x) * rate;
    gimmick->color.y = gimmick->color.y + (goal.y - gimmick->color.y) * rate;
    gimmick->color.z = gimmick->color.z + (goal.z - gimmick->color.z) * rate;
    gimmick->color.w = gimmick->color.w + (goal.w - gimmick->color.w) * rate;
    gimmick->inside = 0;
  }
  if (gimmick->effect != NULL && gimmick->base.typeId == 0x324) {
    GfxEffectSetColorAttached(g_worldEffectMgr, 0x41, NULL, &gimmick->color.x);
    GfxEffectSetColorAttached(g_worldEffectMgr, 0x42, NULL, &gimmick->color.x);
    GfxEffectSetColorAttached(g_worldEffectMgr, 0x43, NULL, &gimmick->color.x);
  }
}
