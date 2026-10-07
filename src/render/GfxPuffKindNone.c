// bdc 0x088294b4 GfxPuffKindNone
#include "bdc.h"

/* Kind 0 handler of the sprite puff (`GfxPuffCtor`) (member-pointer table `0x08ab9f10` indexed
   by the puff's kind byte, run by `GfxPuffUpdate`): does nothing. */
void GfxPuffKindNone(void *puff)
{
    (void)puff;
}
