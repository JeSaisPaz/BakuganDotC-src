// bdc 0x088e9a24 ActorNpcViewConeDtor
#include "bdc.h"

/* Destructor of the view-cone object: when it has an effect manager, stops its six effects
   (`g_actorNpcViewConeEffectIds`) attached to the anchor, then frees it when `flags & 1`. */

void ActorNpcViewConeDtor(void *cone, u32 flags)
{
  ActorNpcViewCone *c = (ActorNpcViewCone *)cone;
  u32 i;

  if (c != NULL) {
    if (c->manager != NULL) {
      for (i = 0; i < 6; i++) {
        GfxEffectStopAttached(c->manager, g_actorNpcViewConeEffectIds[i], (void *)c->anchor);
      }
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(c, NULL, 0);
      MemUnlock();
    }
  }
}
