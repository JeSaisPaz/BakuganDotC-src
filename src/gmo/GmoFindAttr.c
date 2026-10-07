// bdc 0x08a15a50 GmoFindAttr
#include "bdc.h"

/* Returns the `n`-th record of a material's attribute array (0x40-byte GmoAttr records; array
   `attrs`, count `attrCount` in the GmoMaterial owner) whose `type` equals `type`, or NULL. */

void *GmoFindAttr(void *owner, u32 type, int n)
{
  GmoMaterial *mat = (GmoMaterial *)owner;
  GmoAttr *attr;
  int i;

  if (mat != NULL && mat->attrCount > 0) {
    attr = mat->attrs;
    for (i = 0; i < mat->attrCount; i++, attr++) {
      if (attr->type == type && --n < 0) {
        return attr;
      }
    }
  }
  return NULL;
}
