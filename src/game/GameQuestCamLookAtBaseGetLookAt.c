// bdc 0x08a2c894 GameQuestCamLookAtBaseGetLookAt
#include "bdc.h"

/* Entry 2 (`+0x14`) of the quest camera look-at base vtable `0x08af6e98`
   (`GameQuestCamLookAtBaseDtor`), inherited by the eye, look-at and rail springs: returns the
   look-at vector `spring + 0x60`. */

float *GameQuestCamLookAtBaseGetLookAt(void *spring)

{
  return &((GameQuestCamTarget *)spring)->point.x;
}
