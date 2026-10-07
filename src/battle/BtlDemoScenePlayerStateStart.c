// bdc 0x089058d0 BtlDemoScenePlayerStateStart
#include "bdc.h"

/* State 0 of the battle demo scene player task (task id 0x66, 0x70 bytes, vtable `0x08af46a4`,
   `BtlDemoScenePlayerCtor`): advances `state` (to `BtlDemoScenePlayerStateLoad`). */

void BtlDemoScenePlayerStateStart(BtlDemoScenePlayer *self)

{
  self->state++;
}

