// bdc 0x08866428 BtlGetAttributeColor
#include "bdc.h"

/* Returns the RGBA float colour of attribute `attribute` from `g_btlAttributeColors`
   (filled at start-up by `BtlAttributeColorsStaticInit`). Used with
   `BtlBakuganStartColorFlash`. */
float *BtlGetAttributeColor(int attribute)
{
    return g_btlAttributeColors[attribute];
}
