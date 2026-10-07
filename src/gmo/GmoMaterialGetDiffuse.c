// bdc 0x08a15ef4 GmoMaterialGetDiffuse
#include "bdc.h"

/* Returns the diffuse colour of a GMO material's first type-0x82 attribute, or 0. */

u32 GmoMaterialGetDiffuse(GmoMaterial *mat)
{
    GmoAttr *attr;
    int i;

    if (mat != NULL) {
        for (i = 0; i < mat->attrCount; i++) {
            attr = &mat->attrs[i];
            if (attr->type == 0x82) {
                if (attr == NULL) {
                    return 0;
                }
                return attr->diffuse;
            }
        }
    }
    return 0;
}
