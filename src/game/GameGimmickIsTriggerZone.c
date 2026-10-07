// bdc 0x08a2bf78 GameGimmickIsTriggerZone
#include "bdc.h"

/* Returns 0 for the "is a trigger zone" gimmick class test (virtual slot 12, `+0x64`) in
   GameGimmick's vtables. */

int GameGimmickIsTriggerZone(GameGimmick *gimmick)

{
  return 0;
}

