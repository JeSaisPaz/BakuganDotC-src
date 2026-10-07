// bdc 0x0894064c UiScreen390ComputeTerminalAngle
#include "bdc.h"

/* Stores in `terminalAngle` the ground angle (`atan2f(dz, dx)`) from the player
   (`ActorFindPlayer`) to the stage terminal position (`GameStageGetTerminalPosition``(…, 1)`). */

void UiScreen390ComputeTerminalAngle(UiScreen *screen)
{
  Actor *player;
  float pos[4];

  player = (Actor *)ActorFindPlayer();
  GameStageGetTerminalPosition(pos, 1);
  /* the asm round-trips pos through a stack temp with lv.q/sv.q: a bit copy, no effect */
  ((UiScreen390 *)screen)->terminalAngle =
      atan2f(pos[2] - player->base.pos[2], pos[0] - player->base.pos[0]);
}
