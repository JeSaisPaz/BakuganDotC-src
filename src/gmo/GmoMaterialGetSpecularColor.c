// bdc 0x08a15e80 GmoMaterialGetSpecularColor
#include "bdc.h"

/* Returns the specular colour of a GMO material's first type-0x83 attribute, falling back to the
   first type-0x82 (diffuse) attribute's +0x30 word, or 0. */

u32 GmoMaterialGetSpecularColor(GmoMaterial *mat)
{
    GmoAttr *attr;
    int i;

    if (mat != NULL) {
        for (i = 0; i < mat->attrCount; i++) {
            attr = &mat->attrs[i];
            if (attr->type == 0x83) {
                goto found;
            }
        }
        for (i = 0; i < mat->attrCount; i++) {
            attr = &mat->attrs[i];
            if (attr->type == 0x82) {
                goto found;
            }
        }
    }
    return 0;
found:
    if (attr == NULL) {
        return 0;
    }
    return attr->specular;
}
