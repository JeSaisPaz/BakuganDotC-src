// bdc 0x0885dd4c BtlBakuganMaterialSetAlphaRef
#include "bdc.h"

/* Material callback of `BtlBakuganSetupModelShading`: sets the alpha-test reference (the u16 at
   the start of the material state record) to 0xb2 (178). */
void BtlBakuganMaterialSetAlphaRef(void *matState, void *arg)
{
    (void)arg;
    *(u16 *)matState = 0xb2;
}
