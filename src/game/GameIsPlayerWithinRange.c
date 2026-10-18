// bdc 0x088d93cc GameIsPlayerWithinRange
#include "bdc.h"

/* True when the player (`ActorFindPlayer`) is within `radius` (3D distance) of `pos`. */

bool GameIsPlayerWithinRange(float radius, void *unused, const float *pos)
{
  Actor *player = (Actor *)ActorFindPlayer();
  const float *ppos = player->base.pos;
  float dx = VfSub(ppos[0], pos[0]);
  float dy = VfSub(ppos[1], pos[1]);
  float dz = VfSub(ppos[2], pos[2]);
  float dist = VfSqrt(VfDot3(dx, dy, dz, dx, dy, dz));

  return dist <= radius;
}
