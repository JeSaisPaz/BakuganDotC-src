// bdc 0x0886ae30 BtlBakuganSetStateFlag04
#include "bdc.h"

/* Sets flag 4 in the unit flag word `stateFlags`; called by the attack states 7, 8, 10 and 11
   (attack-in-progress marker, exact meaning not confirmed). */
void BtlBakuganSetStateFlag04(BtlBakugan *bakugan)
{
    bakugan->stateFlags |= 4;
}
