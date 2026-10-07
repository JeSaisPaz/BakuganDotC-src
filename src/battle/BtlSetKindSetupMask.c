// bdc 0x08866398 BtlSetKindSetupMask
#include "bdc.h"

/* Stores its argument in `g_btlKindSetupMask`. */
void BtlSetKindSetupMask(u32 value)
{
    g_btlKindSetupMask = value;
}
