// bdc 0x088f47a0 GameFieldCharSetReload
#include "bdc.h"

/* Reloads a room's character set (`GameFieldLoadCharacterSet`) and pushes every queued switch
   event (`+0xcd`, count `+0xcc`) on the pending-event stack. */

void GameFieldCharSetReload(GameFieldCharSet *mgr, s32 a, u8 area, u8 room)
{
  u8 i;

  GameFieldLoadCharacterSet(mgr, a, area, room);
  for (i = 0; i < mgr->queuedCount; i++) {
    GameFieldCharSetPushEvent(mgr, (&mgr->queued[0])[i]);
  }
}
