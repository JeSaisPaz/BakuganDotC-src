// bdc 0x0889fe20 GameGimmickCorePointState02Despawn
#include "bdc.h"

/* State 2 of the core-point gimmick (table `0x08a83c54`): counts the timer `+0x188` down and, when
   it expires, calls `CoreObjectDeferDelete(obj, 0)` on the model (disables it). */

void GameGimmickCorePointState02Despawn(GameGimmickCorePoint *obj)

{
  obj->timer = obj->timer - 1;
  if (obj->timer < 1) {
    CoreObjectDeferDelete((CoreObject *)obj, 0);
  }
}
