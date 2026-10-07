// bdc 0x089d2fa4 NetPdpIsTimedOut
#include "bdc.h"

/* Returns the `CONetPDP` long-silence flag (byte `+0x10`, set by `NetPdpUpdate` after 15 s
   without packets). */

int NetPdpIsTimedOut(NetPdp *self)

{
  return (uint)(self->timedOut != '\0');
}

