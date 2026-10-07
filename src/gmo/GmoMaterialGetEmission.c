// bdc 0x08a15da0 GmoMaterialGetEmission
#include "bdc.h"

/* Returns the emission colour of the first type-0x84 attribute in a GMO material's attribute array,
   or 0. */

u32 GmoMaterialGetEmission(GmoMaterial *mat)
{
    GmoAttr *attr;
    int i;

    if (mat != NULL) {
        for (i = 0; i < mat->attrCount; i++) {
            attr = &mat->attrs[i];
            if (attr->type == 0x84) {
                if (attr == NULL) {
                    return 0;
                }
                return attr->emission;
            }
        }
    }
    return 0;
}
