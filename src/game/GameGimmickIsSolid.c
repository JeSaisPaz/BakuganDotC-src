// bdc 0x08a2bf88 GameGimmickIsSolid
#include "bdc.h"

/* Base gimmick type test of vtable slot 14: returns 0 (not a solid gimmick); the solid gimmick
   overrides it to return 1. */

s32 GameGimmickIsSolid(GameGimmick *gimmick)

{
  return 0;
}

