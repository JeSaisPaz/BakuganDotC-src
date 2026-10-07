// bdc 0x088d9564 GameGetPlayerFacingSectorFrom
#include "bdc.h"

/* Computes the XZ angle from `pos` to the player (`atan2f(dz, dx)`) and classifies it with
   `GameGetPlayerFacingSector`. */

s32 GameGetPlayerFacingSectorFrom(void *gimmick, const float *pos)
{
  Actor *player = (Actor *)ActorFindPlayer();
  float p[4];
  /* the asm copies the player's position quad to the stack (lv.q/sv.q) before reading it */
  p[0] = player->base.pos[0];
  p[1] = player->base.pos[1];
  p[2] = player->base.pos[2];
  p[3] = player->base.pos[3];
  float angle = atan2f(p[2] - pos[2], p[0] - pos[0]);
  return GameGetPlayerFacingSector(angle);
}
