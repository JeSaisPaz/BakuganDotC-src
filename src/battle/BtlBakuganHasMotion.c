// bdc 0x08862c4c BtlBakuganHasMotion
#include "bdc.h"

/* Returns whether the unit's kind defines logical motion `motion` (its entry in the motion table
   is not 0xffff). */
int BtlBakuganHasMotion(BtlBakugan *bakugan, int motion)
{
    return (u16)bakugan->motionTable[motion] != 0xffff;
}
