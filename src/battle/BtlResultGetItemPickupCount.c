// bdc 0x0883f4dc BtlResultGetItemPickupCount
#include "bdc.h"

/* Result-screen score item 0x17: how many items the player's battle Bakugan has picked up
   (`BtlBakugan` `itemPickupCount`, counted by `BtlItemApplyPickup`); 0 when there is no
   player Bakugan. */
int BtlResultGetItemPickupCount(void *hud)
{
    BtlBakugan *player = (BtlBakugan *)BtlHudGetPlayerBakugan(hud);
    int count = 0;

    if (player != NULL) {
        count = player->itemPickupCount;
    }
    return count;
}
