// bdc 0x088dfe5c ActorSetGuardsViewConeVisible
#include "bdc.h"

/* Writes `value` to the visibility byte `+0x25` of the view-cone object (`+0x418`,
   `ActorNpcViewConeCtor`) of every actor whose placement record marks it as a guard (`+0x37`). */

void ActorSetGuardsViewConeVisible(u8 value)

{
  Actor *actor;

  for (actor = *(Actor **)g_actorList; actor != (Actor *)0x0; actor = (Actor *)actor->base.base.next) {
    if (((ActorNpcPlacement *)actor->placement)->placed != 0) {
      ((ActorNpcViewCone *)((ActorNpc *)actor)->viewCone)->visible = value;
    }
  }
}
