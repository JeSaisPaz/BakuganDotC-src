// bdc 0x088db9b8 GameGimmickSwitchCheckPlayerFacing
#include "bdc.h"

/* While active, clears bit 1 (mask 0x2) of `+0x15f`, then sets it when a player exists
   (`ActorFindPlayer`), is within 21 units (`GameIsPlayerWithinRange`) and faces the switch
   (`GameGetPlayerFacingSectorFrom` returns 1). */

void GameGimmickSwitchCheckPlayerFacing(GameGimmickSwitch *gimmick)
{
  float a[4];
  float b[4];

  if (gimmick->base.active != 0) {
    gimmick->base.contactFlags &= 0xfd;
    if (ActorFindPlayer() != 0) {
      a[0] = gimmick->base.base.pos[0];
      a[1] = gimmick->base.base.pos[1];
      a[2] = gimmick->base.base.pos[2];
      a[3] = gimmick->base.base.pos[3];
      if (GameIsPlayerWithinRange(21.0f, gimmick, a) == 1) {
        b[0] = gimmick->base.base.pos[0];
        b[1] = gimmick->base.base.pos[1];
        b[2] = gimmick->base.base.pos[2];
        b[3] = gimmick->base.base.pos[3];
        if (GameGetPlayerFacingSectorFrom(gimmick, b) == 1) {
          gimmick->base.contactFlags |= 2;
        }
      }
    }
  }
}
