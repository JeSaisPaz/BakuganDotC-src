// bdc 0x08a2a4cc BtlAiParamsGetSidestepMode
#include "bdc.h"

/* Default per-species AI parameter getter (entry 2, fn at `+0x14`): returns sidestep mode 1. */
s32 BtlAiParamsGetSidestepMode(BtlAiParams *self)
{
    (void)self;
    return 1;
}
