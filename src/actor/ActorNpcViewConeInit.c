// bdc 0x088e9ac8 ActorNpcViewConeInit
#include "bdc.h"

/* Binds the view cone to an effect manager and an anchor position (the NPC's `+0x20`) and spawns
   its six attached effects (`g_actorNpcViewConeEffectIds`, `GfxEffectSpawnAttached`), each
   with direction/colour `g_vecX` (`effect+0x90`); the mesh object's `visible` is cleared.
   Marks it initialised (`+8`). */

void ActorNpcViewConeInit(void *cone, void *effectMgr, const float *anchor)
{
  ActorNpcViewCone *c = (ActorNpcViewCone *)cone;
  u32 i;

  c->manager = (GfxEffectMgr *)effectMgr;
  c->anchor = anchor;
  for (i = 0; i < 6; i++) {
    GfxEffect *effect = (GfxEffect *)GfxEffectSpawnAttached(c->manager, g_actorNpcViewConeEffectIds[i], (float *)c->anchor);
    c->effects[i] = effect;
    effect->dir[0] = g_vecX.x;
    effect->dir[1] = g_vecX.y;
    effect->dir[2] = g_vecX.z;
    effect->dir[3] = g_vecX.w;
    if (effect->meshObj != NULL) {
      effect->meshObj->visible = 0;
    }
  }
  c->flag08 = 1;
}
