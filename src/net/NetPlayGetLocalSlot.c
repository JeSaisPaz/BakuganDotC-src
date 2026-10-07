// bdc 0x0881b62c NetPlayGetLocalSlot
#include "bdc.h"

/* Returns the local player's slot in the session (`localSlot`, set by
   NetPlayState7Session; -1 before). */
s32 NetPlayGetLocalSlot(NetPlay *self)
{
    return self->localSlot;
}
