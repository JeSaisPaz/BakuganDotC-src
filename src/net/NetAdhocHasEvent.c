// bdc 0x089d495c NetAdhocHasEvent
#include "bdc.h"

/* Returns whether adhocctl event `event` has been seen (bit `event` of the mask `+0xc`, set by
   `NetAdhocOnCtlEvent`). */

bool NetAdhocHasEvent(NetAdhocConn *self, s32 event)

{
  return (self->eventMask & 1 << (event & 0x1fU)) != 0;
}

