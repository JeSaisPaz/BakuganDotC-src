// bdc 0x08a2c3f0 GameGimmickSwitchIsSwitch
#include "bdc.h"

/* Returns 1 for the "is a switch" gimmick class test (virtual slot 10, `+0x54`) in
   GameGimmickSwitch's vtable. */

int GameGimmickSwitchIsSwitch(GameGimmickSwitch *gimmick)

{
  return 1;
}

