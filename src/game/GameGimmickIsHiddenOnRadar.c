// bdc 0x08a2c18c GameGimmickIsHiddenOnRadar
#include "bdc.h"

/* Gimmick vtable entry 17 (`+0x8c`), root implementation shared by every field gimmick except the
   core point: returns false, i.e. the gimmick's blip is shown on the field HUD radar. */

bool GameGimmickIsHiddenOnRadar(GameGimmick *gimmick)

{
  return false;
}

