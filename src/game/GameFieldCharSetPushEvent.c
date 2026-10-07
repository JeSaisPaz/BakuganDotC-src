// bdc 0x088f4784 GameFieldCharSetPushEvent
#include "bdc.h"

/* Pushes `id` on the pending-event stack `+0x89` (depth `+0xcb`). */

void GameFieldCharSetPushEvent(GameFieldCharSet *mgr, u8 id)
{
  u8 depth;

  depth = mgr->eventDepth;
  mgr->events[depth] = id;
  mgr->eventDepth = depth + 1;
}
