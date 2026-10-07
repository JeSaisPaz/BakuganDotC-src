// bdc 0x088602ac BtlBakuganIsAirborne
#include "bdc.h"

/* Returns 1 when the unit has been airborne for more than 3 frames (`+0x184`, counted by
   `BtlBakuganApplyGravity`) and, if `checkHeight` is set, its height `+0x24` minus the hover
   height `stats+0x70` is more than 100 above the ground point `+0x344`. */
int BtlBakuganIsAirborne(BtlBakugan *bakugan, char checkHeight)
{
    float groundY;
    float bottomY;

    if (bakugan->airborneFrames < 4) {
        return 0;
    }
    if (checkHeight == 0) {
        return 1;
    }
    groundY = bakugan->groundY + 100.0f;
    bottomY = bakugan->base.pos[1] - bakugan->combat.stats->hoverHeight;
    return groundY < bottomY ? 1 : 0;
}
