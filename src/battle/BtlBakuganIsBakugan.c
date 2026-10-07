// bdc 0x08a2a16c BtlBakuganIsBakugan
#include "bdc.h"

/* Returns 1 for the "is a Bakugan" (BtlBakugan and its CPU subclasses) battle-unit class test
   (virtual slot 10, `+0x54`) in BtlBakugan's vtables. */
int BtlBakuganIsBakugan(BtlBakugan *self)
{
    (void)self;
    return 1;
}
