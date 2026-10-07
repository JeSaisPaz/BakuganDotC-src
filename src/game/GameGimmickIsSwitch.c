// bdc 0x08a2bf68 GameGimmickIsSwitch
#include "bdc.h"

/* Returns 0 for the "is a switch" gimmick class test (virtual slot 10, `+0x54`) in GameGimmick's
   vtables. */

int GameGimmickIsSwitch(GameGimmick *gimmick)

{
  return 0;
}

