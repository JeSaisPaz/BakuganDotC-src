// bdc 0x08a15f3c GmoMaterialGetSpecularPower
#include "bdc.h"

/* Returns the specular power (float word `word3c`) of a GMO material's first type-0x83 attribute,
   falling back to the first type-0x82 attribute, or 0.0f. */

float GmoMaterialGetSpecularPower(void *mat)
{
  GmoMaterial *m = (GmoMaterial *)mat;
  GmoAttr *a;
  int i;

  if (m == NULL) {
    return 0.0f;
  }
  for (i = 0, a = m->attrs; i < (int)m->attrCount; i++, a++) {
    if (a->type == 0x83) {
      return *(float *)&a->word3c;
    }
  }
  for (i = 0, a = m->attrs; i < (int)m->attrCount; i++, a++) {
    if (a->type == 0x82) {
      return *(float *)&a->word3c;
    }
  }
  return 0.0f;
}
