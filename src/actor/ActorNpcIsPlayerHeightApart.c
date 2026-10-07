// bdc 0x088e6184 ActorNpcIsPlayerHeightApart
#include "bdc.h"

/* Returns 1 when the vertical distance between the player and the NPC is at least
   `g_actorNpcHeightApartMin` (different floor: the NPC cannot see the player), else 0. */

s32 ActorNpcIsPlayerHeightApart(ActorNpc *self)
{
  Actor *player;
  float dy;

  player = (Actor *)ActorFindPlayer();
  dy = player->base.pos[1] - self->base.base.pos[1];
  if (dy < 0.0f) {
    dy = -dy;
  }
  if (g_actorNpcHeightApartMin <= dy) {
    return 1;
  }
  return 0;
}
