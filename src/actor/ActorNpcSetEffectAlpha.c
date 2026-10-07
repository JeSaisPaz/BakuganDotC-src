// bdc 0x088e6068 ActorNpcSetEffectAlpha
#include "bdc.h"

/* Sets the colour of effect `effectId` on the NPC's head anchor (`+0x1b0`, or `+0x3f0` with a head
   object) to white with alpha `alpha` (`GfxEffectSetColorAttached`). */

void ActorNpcSetEffectAlpha(float alpha, ActorNpc *self, s32 effectId)

{
  float *attach;
  float rgba[4] __attribute__((aligned(16)));
  
  attach = (self->base).mtx + 0xc;
  if (self->head != (void *)0x0) {
    attach = self->headPos;
  }
  rgba[0] = 1.0f;
  rgba[1] = 1.0f;
  rgba[2] = 1.0f;
  rgba[3] = alpha;
  GfxEffectSetColorAttached(g_worldEffectMgr,effectId,attach,rgba);
  return;
}

