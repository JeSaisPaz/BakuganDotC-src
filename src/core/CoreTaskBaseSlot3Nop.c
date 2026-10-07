// bdc 0x08a29614 CoreTaskBaseSlot3Nop
#include "bdc.h"

/* Empty virtual in slot 3 (`vt+0x1c`) of the `CoreTask` base vtable `0x08af5224`.
   No task class overrides it and no known caller dispatches slot 3; its intended role is unknown. */
void CoreTaskBaseSlot3Nop(CoreTask *task)
{
    (void)task;
}
