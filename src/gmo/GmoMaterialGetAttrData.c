// bdc 0x08a15fb8 GmoMaterialGetAttrData
#include "bdc.h"

/* Returns the data block (`data`, `+0x28`) of an attribute of a `GmoMaterial`, or NULL for a
   NULL material. `index` is a GMO attribute reference: when `index + 1` fits in 16 bits it
   indexes `attrs` (NULL when `index & 0xffff` is not below `attrCount`; the record address uses
   the full `index`), otherwise it is the address of a `GmoAttr` record itself; a NULL record
   gives NULL. */
void *GmoMaterialGetAttrData(GmoMaterial *mat, u32 index)
{
    GmoAttr *attr;

    if (mat == NULL) {
        return NULL;
    }
    if (((index + 1) & 0xffff0000) == 0) {
        if ((index & 0xffff) >= mat->attrCount) {
            return NULL;
        }
        attr = &mat->attrs[index];
    } else {
        attr = (GmoAttr *)(uintptr_t)index;
    }
    if (attr == NULL) {
        return NULL;
    }
    return attr->data;
}
