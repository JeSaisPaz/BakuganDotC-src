// bdc 0x088e1448 ActorPlayerGetByte3ae
#include "bdc.h"

/* Returns the player's `triggerInReach` byte (`+0x3ae`): non-zero when a field trigger is within
   reach (cleared each `ActorPlayerUpdate`, set by `GameFieldCheckTriggers` for a reachable
   trigger). Read by `UiFieldHudStage32Phase` for the field HUD prompt. */
u8 ActorPlayerGetByte3ae(ActorPlayer *self)
{
    return self->triggerInReach;
}
