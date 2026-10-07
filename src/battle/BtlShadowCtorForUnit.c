// bdc 0x0888537c BtlShadowCtorForUnit
#include "bdc.h"

/* Constructs a battle unit's shadow object (owner `unit`, mode 0: smoke column) and initialises it
   (`BtlShadowInit`). Called by `BtlBakuganCtor`, which stores it at `unit+0x200`. */

BtlShadow *BtlShadowCtorForUnit(BtlShadow *shadow, void *unit)
{
    shadow->owner = unit;
    shadow->mode = 0;
    BtlShadowInit(shadow);
    return shadow;
}
