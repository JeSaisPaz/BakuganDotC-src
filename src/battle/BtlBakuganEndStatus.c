// bdc 0x08865978 BtlBakuganEndStatus
#include "bdc.h"

/* Ends status `which` of a unit: stops its visual effects (`BtlBakuganStopStatusEffects`), then
   clears the matching combat status (`BtlBakuganClearStatusById`). */
void BtlBakuganEndStatus(BtlBakugan *self, int which)
{
    BtlBakuganStopStatusEffects(self, which);
    BtlBakuganClearStatusById(self, which);
}
