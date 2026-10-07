// bdc 0x089407c4 UiScreen390SpawnEffect
#include "bdc.h"

/* Spawns effect `effect` (`GfxEffectSpawn` on `g_worldEffectMgr`) next to the player, `dist`
   units along the cardinal direction nearest to the terminal angle `terminalAngle` (-Z for
   [-2.35, -0.78], +X for [-0.78, 0.78], +Z for [0.78, 2.35], -X otherwise), and keeps it in
   `effect`. */

void UiScreen390SpawnEffect(float dist, UiScreen *screen, s32 effect)

{
  UiScreen390 *self = (UiScreen390 *)screen;
  Actor *player;
  float pos[4];

  player = (Actor *)ActorFindPlayer();
  /* quad copy of the player position */
  pos[0] = player->base.pos[0];
  pos[1] = player->base.pos[1];
  pos[2] = player->base.pos[2];
  pos[3] = player->base.pos[3];
  if (!(self->terminalAngle < -2.35f) && self->terminalAngle <= -0.78f) {
    pos[2] = pos[2] - dist;
  }
  else if (!(self->terminalAngle < -0.78f) && self->terminalAngle <= 0.78f) {
    pos[0] = pos[0] + dist;
  }
  else if (!(self->terminalAngle < 0.78f) && self->terminalAngle <= 2.35f) {
    pos[2] = pos[2] + dist;
  }
  else {
    pos[0] = pos[0] - dist;
  }
  self->effect = GfxEffectSpawn(g_worldEffectMgr, effect, pos);
}
