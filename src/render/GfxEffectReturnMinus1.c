// bdc 0x0881d968 GfxEffectReturnMinus1
#include "bdc.h"

/* Returns -1. Called from GfxEffectRunCommands (likely a stubbed lookup that always reports
   'none'). */
s32 GfxEffectReturnMinus1(void)
{
    return -1;
}
