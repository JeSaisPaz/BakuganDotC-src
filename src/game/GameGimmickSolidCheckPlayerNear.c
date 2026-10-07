// bdc 0x088dadd8 GameGimmickSolidCheckPlayerNear
#include "bdc.h"

/* While the gimmick is active (`+0x15e`), clears bit 1 (mask 0x2) of `+0x15f`, then sets it when the
   player is within 15 units of it (`GameIsPlayerWithinRange`). */

void GameGimmickSolidCheckPlayerNear(GameGimmick *gimmick)
{
  if (gimmick->active != 0) {
    const GameGimmickRecord *rec;
    float scaled[4];
    float pos[4];

    gimmick->contactFlags &= 0xfd;
    rec = gimmick->record;
    /* 20 x the record's half-size (1/4096 units), minimum 12: computed but never used.
       Lane 3 of the vscl.t result is stale and never read, so it is left out. */
    scaled[0] = ((float)rec->extent[0] * 0.00024414062f) * 20.0f;
    scaled[1] = ((float)rec->extent[1] * 0.00024414062f) * 20.0f;
    scaled[2] = ((float)rec->extent[2] * 0.00024414062f) * 20.0f;
    if (scaled[0] < 12.0f) {
      scaled[0] = 12.0f;
    }
    (void)scaled;
    pos[0] = gimmick->base.pos[0];
    pos[1] = gimmick->base.pos[1];
    pos[2] = gimmick->base.pos[2];
    pos[3] = gimmick->base.pos[3];
    if (GameIsPlayerWithinRange(15.0f, gimmick, pos)) {
      gimmick->contactFlags |= 2;
    }
  }
}
