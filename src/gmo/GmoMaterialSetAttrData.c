// bdc 0x08a1626c GmoMaterialSetAttrData
#include "bdc.h"

/* Sets the 16-byte data block (`data` field) of attribute `index` of a material's attribute array
   (0x40-byte records; array `attrs`, count `attrCount` of the owner) to a copy of `data`
   (allocating it from pool 0 on demand, flag 0x800000 in `flags`); NULL releases it. The second
   argument is an index when its low half is 0xffff-biased like `GmoFindAttr` callers pass,
   otherwise an attribute pointer. */

void GmoMaterialSetAttrData(void *mat, u32 index, const u32 *data)
{
  GmoMaterial *m = (GmoMaterial *)mat;
  GmoAttr *a = (GmoAttr *)(uintptr_t)index;
  u32 *dst;

  if (m == NULL) {
    return;
  }
  if (((index + 1) & 0xffff0000) == 0) {
    if ((index & 0xffff) >= m->attrCount) {
      return;
    }
    a = &m->attrs[index];
  }
  if (a == NULL) {
    return;
  }
  if (data == NULL) {
    GmoHeapReleaseThunk(0, a->data);
    a->data = NULL;
    a->flags &= 0xff7fffff;
    return;
  }
  dst = (u32 *)a->data;
  if (dst == NULL) {
    dst = GmoHeapAlloc(0, 0x10, 0x10);
    a->data = dst;
    if (dst == NULL) {
      return;
    }
    a->flags |= 0x800000;
  }
  {
    u32 w0 = data[0], w1 = data[1], w2 = data[2];
    dst[3] = data[3];
    dst[0] = w0;
    dst[1] = w1;
    dst[2] = w2;
  }
}
