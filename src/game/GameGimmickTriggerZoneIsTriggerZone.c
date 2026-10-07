// bdc 0x08a2c3d0 GameGimmickTriggerZoneIsTriggerZone
#include "bdc.h"

/* Returns 1 for the "is a trigger zone" gimmick class test (virtual slot 12, `+0x64`) in
   GameGimmickTriggerZone's vtable. */

int GameGimmickTriggerZoneIsTriggerZone(GameGimmickTriggerZone *gimmick)

{
  return 1;
}

