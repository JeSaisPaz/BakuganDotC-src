// bdc 0x088663ec BtlBakuganGetAttributeColor
#include "bdc.h"

/* Returns the RGBA colour of the unit's attribute in `g_btlAttributeColors`, where the attribute
   is the unit's virtual slot 20 (`+0xa0`; base implementation: signed byte `stats[0]`). Used by the
   ability/finish states for flashes and effect tints. */
float *BtlBakuganGetAttributeColor(BtlBakugan *self)
{
    const VtblEntry *entry = &((const VtblEntry *)self->base.base.vtable)[20];
    int attribute = ((int (*)(void *))entry->fn)((u8 *)self + entry->delta);

    return g_btlAttributeColors[attribute];
}
