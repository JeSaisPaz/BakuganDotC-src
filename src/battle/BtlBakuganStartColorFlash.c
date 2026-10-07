// bdc 0x08863104 BtlBakuganStartColorFlash
#include "bdc.h"

/* Starts a colour flash on the unit: copies the 4-float colour to `flashColor`, then overwrites
   its last component with the strength `strength`; `BtlBakuganUpdateColorFlash` fades it out.
   Callers pass e.g. the attribute colour from `BtlGetAttributeColor`. */
void BtlBakuganStartColorFlash(float strength, BtlBakugan *bakugan, float *color)
{
    bakugan->flashColor[0] = color[0];
    bakugan->flashColor[1] = color[1];
    bakugan->flashColor[2] = color[2];
    bakugan->flashColor[3] = color[3];
    bakugan->flashColor[3] = strength;
}
