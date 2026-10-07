// bdc 0x088ed238 GameEventOp28CamResetKeys
#include "bdc.h"

/* Handler of event opcode 0x28 (`GameEventExecCommand`): copies the captured home keys back into
   the end keys: eye `+0x24..` -> `+0xc..`, target `+0x54..` -> `+0x3c..`, fov `+0x66` -> `+0x62`.
    */

void GameEventOp28CamResetKeys(GameEvent *self, u8 flag, s16 arg)

{
  GameEventCamKeys *keys;
  s32 y;
  s32 x;
  
  keys = self->camKeys;
  x = keys->eyeOrig[1];
  y = keys->eyeOrig[2];
  keys->eyeEnd[0] = keys->eyeOrig[0];
  keys->eyeEnd[1] = x;
  keys->eyeEnd[2] = y;
  keys = self->camKeys;
  x = keys->targetOrig[1];
  y = keys->targetOrig[2];
  keys->targetEnd[0] = keys->targetOrig[0];
  keys->targetEnd[1] = x;
  keys->targetEnd[2] = y;
  self->camKeys->fovEnd = self->camKeys->fovOrig;
  return;
}

