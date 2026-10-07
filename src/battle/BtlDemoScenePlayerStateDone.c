// bdc 0x0890785c BtlDemoScenePlayerStateDone
#include "bdc.h"

/* State 3 of the battle demo scene player task (task id 0x66, 0x70 bytes, vtable `0x08af46a4`,
   `BtlDemoScenePlayerCtor`): advances `state` past the end of the table (playback
   finished). */

void BtlDemoScenePlayerStateDone(BtlDemoScenePlayer *self)

{
  self->state++;
}

