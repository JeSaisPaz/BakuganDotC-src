// bdc 0x088f4be4 GameFieldCharSetPopEvent
#include "bdc.h"

/* Pops the pending-event stack `events`; returns -1 when empty. */

s32 GameFieldCharSetPopEvent(GameFieldCharSet *mgr)
{
  if (mgr->eventDepth > 0) {
    mgr->eventDepth = mgr->eventDepth - 1;
    return (s8)mgr->events[mgr->eventDepth];
  }
  return -1;
}
