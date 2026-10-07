// bdc 0x08866354 BtlSetAnimPhaseCounter
#include "bdc.h"

/* Stores its argument in `g_btlAnimPhaseCounter`. */
void BtlSetAnimPhaseCounter(s32 value)
{
    g_btlAnimPhaseCounter = value;
}
