// bdc 0x08a2c3e0 GameGimmickTouchSpotIsTouchSpot
#include "bdc.h"

/* Returns 1 for the "is a touch spot" gimmick class test (virtual slot 13, `+0x6c`) in
   GameGimmickTouchSpot's vtable. */

int GameGimmickTouchSpotIsTouchSpot(GameGimmickTouchSpot *gimmick)

{
  return 1;
}

