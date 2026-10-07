// bdc 0x088f4628 GameFieldCharSetLoadFresh
#include "bdc.h"

/* Loads the character set of area `area`, room `room` (`GameFieldLoadCharacterSet`, `reuse`
   passed through) and clears the queued switch events: `queuedCount` = 0 and 10 bytes of `queued`
   zeroed. Called by `GameFieldPhaseLoad`. */

void GameFieldCharSetLoadFresh(void *mgr, s32 area, u8 room, u8 reuse)

{
  GameFieldCharSet *cs = (GameFieldCharSet *)mgr;

  GameFieldLoadCharacterSet(mgr,area,room,reuse);
  cs->queuedCount = 0;
  memset(cs->queued,0,10);
}
