// bdc 0x088dfeac ActorNotifyNearestGuardOfNoise
#include "bdc.h"

/* Among the guard actors (placement `+0x37`) whose virtual slot 33 accepts, finds the one nearest
   to the noise (`ActorDistSqTo(actor, pos)`, start 250000) and calls its slot 17
   (`maybe_ActorNpcHearNoise`) with `(pos, arg)`. */

void ActorNotifyNearestGuardOfNoise(const float *pos, s32 arg)

{
  Actor *actor;
  Actor *best = (Actor *)0x0;
  float bestDist = 250000.0f;

  for (actor = *(Actor **)g_actorList; actor != (Actor *)0x0; actor = (Actor *)actor->base.base.next) {
    if (((ActorNpcPlacement *)actor->placement)->placed != 0) {
      const VtblEntry *accept = &((const VtblEntry *)actor->base.base.vtable)[33];

      if (((s32(*)(void *))accept->fn)((u8 *)actor + accept->delta) != 0) {
        float dist = ActorDistSqTo(actor, pos);

        if (best == (Actor *)0x0 || dist < bestDist) {
          bestDist = dist;
          best = actor;
        }
      }
    }
  }
  if (best != (Actor *)0x0) {
    const VtblEntry *hear = &((const VtblEntry *)best->base.base.vtable)[17];

    ((void (*)(void *, const float *, s32))hear->fn)((u8 *)best + hear->delta, pos, arg);
  }
  return;
}
