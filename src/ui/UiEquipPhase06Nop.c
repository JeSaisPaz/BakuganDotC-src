// bdc 0x0895c7ec UiEquipPhase06Nop
#include "bdc.h"

/* Empty phase 6 (last entry of the phase table `0x08a9d5f8`) of the loadout screen
   (`UiEquipCtor`): just `jr ra`. */
void UiEquipPhase06Nop(UiEquip *self)
{
    (void)self;
}
