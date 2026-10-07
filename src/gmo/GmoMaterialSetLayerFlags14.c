// bdc 0x08a16040 GmoMaterialSetLayerFlags14
#include "bdc.h"

/* Sets the bits `mask` of `layerFlags14` of the first attribute of a material to `value` and mirrors
   whether any bit remains set into the low half of `flags` (0xffff or 0). */

void GmoMaterialSetLayerFlags14(GmoMaterial *mat, u16 mask, u16 value)
{
    GmoAttr *attr;
    u16 flags14;

    if (mat != NULL && mat->attrCount != 0) {
        attr = mat->attrs;
        if (attr != NULL) {
            flags14 = ~mask & attr->layerFlags14 | mask & value;
            attr->layerFlags14 = flags14;
            if (flags14 != 0) {
                attr->flags |= 0xffff;
                return;
            }
            attr->flags &= 0xffff0000;
        }
    }
}
