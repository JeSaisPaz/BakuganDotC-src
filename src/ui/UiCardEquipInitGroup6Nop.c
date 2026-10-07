// bdc 0x0896be20 UiCardEquipInitGroup6Nop
#include "bdc.h"

/* Empty init step of `UiCardEquip` for sprite group 6 (layout pair `+0x2b78`);
   called from step 0 of `UiCardEquipPhaseMain` between the other group initialisers and does
   nothing. */
void UiCardEquipInitGroup6Nop(UiCardEquip *self)
{
    (void)self;
}
