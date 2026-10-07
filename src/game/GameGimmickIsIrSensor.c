// bdc 0x08a2bf90 GameGimmickIsIrSensor
#include "bdc.h"

/* Base gimmick type test of vtable slot 15: returns 0 (not an IR sensor); the IR sensor gimmick
   overrides it to return 1. */

s32 GameGimmickIsIrSensor(GameGimmick *gimmick)

{
  return 0;
}

