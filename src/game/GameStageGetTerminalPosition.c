// bdc 0x088d45ec GameStageGetTerminalPosition
#include "bdc.h"

/* Writes the position of the current stage point `g_gameStagePoint` into `out` (4 floats): from
   `GameFieldPointGetPos` when `alt` is 0, else from `ActorStageObjRecordGetPos` with kind
   `point + 0xce`. When x, y and z of the found position are all 0 (not found), `out` is zeroed. */

void GameStageGetTerminalPosition(float *out, s32 alt)
{
  float pos[4];
  float tmp[4];

  if (alt == 0) {
    GameFieldPointGetPos(tmp, g_gameStagePoint);
  } else {
    ActorStageObjRecordGetPos(tmp, g_gameStagePoint + 0xce);
  }
  pos[0] = tmp[0];
  pos[1] = tmp[1];
  pos[2] = tmp[2];
  pos[3] = tmp[3];
  if (pos[0] != 0.0f || pos[1] != 0.0f || pos[2] != 0.0f) {
    out[0] = pos[0];
    out[1] = pos[1];
    out[2] = pos[2];
    out[3] = pos[3];
    return;
  }
  out[2] = 0.0f;
  out[1] = 0.0f;
  out[0] = 0.0f;
  out[3] = 0.0f;
}
