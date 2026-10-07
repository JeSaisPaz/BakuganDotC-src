// bdc 0x08850b50 BtlMainGetLocalSlot
#include "bdc.h"

/* Returns the local player slot of the battle main object. */
s32 BtlMainGetLocalSlot(BtlMain *main)
{
    return main->localSlot;
}
