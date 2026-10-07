// bdc 0x0882c508 BtlHudGetPlayerBakugan
#include "bdc.h"

/* HUD helper returning the player's battle Bakugan: a wrapper that ignores
   `self` and returns BtlGetPlayerBakugan() (NULL when there is none). */
void *BtlHudGetPlayerBakugan(BtlHud *self)
{
    (void)self;
    return BtlGetPlayerBakugan();
}
