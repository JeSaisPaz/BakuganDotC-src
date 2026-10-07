// bdc 0x088e13c0 ActorPlayerIsStealthed
#include "bdc.h"

/* Returns the stealth flag +0x3a0 of the player. */
u8 ActorPlayerIsStealthed(ActorPlayer *self)
{
    return self->stealth;
}
