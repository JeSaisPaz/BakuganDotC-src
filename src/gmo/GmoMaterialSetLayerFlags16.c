// bdc 0x08a16004 GmoMaterialSetLayerFlags16
#include "bdc.h"

/* Sets the bits `mask` of the half-word `+0x16` of the first record of a material's attribute array
   (0x40-byte records, type half-word at `+8`; array at `+8`, count `+0xc` of the owner) to `value`.
    */

void GmoMaterialSetLayerFlags16(void *mat, u16 mask, u16 value)
{
  GmoMaterial *m = (GmoMaterial *)mat;
  GmoAttr *attr;

  if ((m != NULL) && (m->attrCount != 0)) {
    attr = m->attrs;
    if (attr != NULL) {
      attr->layerFlags16 = (~mask & attr->layerFlags16) | (mask & value);
    }
  }
}
