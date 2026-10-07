// bdc 0x088e1440 ActorPlayerGetByte3ad
#include "bdc.h"

/* Returns the player's `talkTargetInReach` byte (`+0x3ad`): non-zero when a talk target is
   within reach (cleared each `ActorPlayerUpdate`, set when `ActorPlayerFindTalkTarget` finds
   one). Read by `UiFieldHudStage32Phase` for the field HUD prompt. */
u8 ActorPlayerGetByte3ad(ActorPlayer *self)
{
    return self->talkTargetInReach;
}
