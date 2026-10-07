// bdc 0x088e5bac ActorNpcShowHeadEffect
#include "bdc.h"

/* Starts or stops an effect attached over an NPC's head (anchor `+0x1b0`, or `+0x3f0` when the NPC
   has the alternate head object `+0x3e0`): `clearAll` first stops every effect on the anchor
   (`GfxEffectStopAttached(g_worldEffectMgr, -1, anchor)`), then `show` spawns effect `effectId`
   (`GfxEffectSpawnAttached`) or, when clear, stops it. */

void ActorNpcShowHeadEffect(ActorNpc *self, s32 effectId, s8 show, s8 clearAll)
{
  float *attach;

  attach = (self->base).mtx + 0xc;
  if (self->head != NULL) {
    attach = self->headPos;
  }
  if (attach != NULL) {
    if (clearAll != 0) {
      GfxEffectStopAttached(g_worldEffectMgr, -1, attach);
    }
    if (show == 0) {
      GfxEffectStopAttached(g_worldEffectMgr, effectId, attach);
    } else if (effectId != -1) {
      GfxEffectSpawnAttached(g_worldEffectMgr, effectId, attach);
    }
  }
}
