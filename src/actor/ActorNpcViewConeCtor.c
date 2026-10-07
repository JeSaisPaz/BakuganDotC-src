// bdc 0x088e99e8 ActorNpcViewConeCtor
#include "bdc.h"

/* Constructor of the 0x28-byte view-cone object of the NPCs (`npc+0x418`): clears the effect
   manager/anchor and the six effect handles `+0xc..+0x20`, sets `+0x24` and the visibility byte
   `+0x25` to 1. */

void *ActorNpcViewConeCtor(void *cone)
{
  ActorNpcViewCone *c = (ActorNpcViewCone *)cone;
  u32 i;

  c->manager = NULL;
  c->flag08 = 0;
  for (i = 0; i < 6; i++) {
    c->effects[i] = NULL;
  }
  c->anchor = NULL;
  c->flag24 = 1;
  c->visible = 1;
  return cone;
}
