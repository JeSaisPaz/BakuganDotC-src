// bdc 0x08864220 BtlBakuganIsObjectInFront
#include "bdc.h"

/* `BtlBakuganIsPointInFront` on the position (`+0x20`) of `other`. */
int BtlBakuganIsObjectInFront(BtlBakugan *bakugan, void *other)
{
    return BtlBakuganIsPointInFront(bakugan, ((GfxModel *)other)->pos);
}
