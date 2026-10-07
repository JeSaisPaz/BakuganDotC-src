// bdc 0x08a15de8 GmoMaterialGetReflection
#include "bdc.h"

/* Returns the reflection value of a GMO material: byte `+0x3b` (high byte of `word38`) of its first
   type-0x86 attribute times 1/255 (`g_gmoInv255`), or 0. */

float GmoMaterialGetReflection(void *mat)
{
  GmoMaterial *m = (GmoMaterial *)mat;
  GmoAttr *a;
  int i;
  u32 v = 0;

  if (m == NULL) {
    return 0.0f;
  }
  for (i = 0, a = m->attrs; i < (int)m->attrCount; i++, a++) {
    if (a->type == 0x86) {
      v = (u8)(a->emission >> 24);
      break;
    }
  }
  return (float)(s32)v * g_gmoInv255;
}
