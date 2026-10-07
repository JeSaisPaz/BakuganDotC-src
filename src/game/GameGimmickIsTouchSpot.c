// bdc 0x08a2bf80 GameGimmickIsTouchSpot
#include "bdc.h"

/* Returns 0 for the "is a touch spot" gimmick class test (virtual slot 13, `+0x6c`) in
   GameGimmick's vtables. */

int GameGimmickIsTouchSpot(GameGimmick *gimmick)

{
  return 0;
}

