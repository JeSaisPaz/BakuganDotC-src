// bdc 0x08a160a0 GmoMaterialCloneAttrs
#include "bdc.h"

/* Appends one attribute record of `type` to a material's attribute array (`attrs`/`attrCount`,
   0x40-byte GmoAttr records): measures `count + 1` records in a fresh private plan
   (`GmoAttrMeasureCopy`), commits, carves them (`GmoPlanTakeAttrs`), copies the old records
   (`GmoAttrCopy`), releases the old array (`GmoAttrArrayRelease`) and initialises the new last
   record's type, `flags` (from g_gmoAttrTypeFlags), layer bit 0 and `blendDst`. Returns the new
   record, or NULL if `mat` is NULL or the plan cannot be committed. */

void *GmoMaterialCloneAttrs(void *mat, int type)
{
  GmoMaterial *m = (GmoMaterial *)mat;
  GmoImagePlan plan;
  GmoAttr *old;
  GmoAttr *fresh;
  GmoAttr *rec;
  int count;
  int n;
  int i;
  u32 flags;

  if (m == NULL) {
    return NULL;
  }
  count = m->attrCount;
  old = m->attrs;
  n = count + 1;
  GmoPlanInit(&plan);
  GmoPlanReserveAttrs(n, &plan);
  for (i = 0; i < count; i++) {
    GmoAttrMeasureCopy(NULL, &old[i], 1, &plan);
  }
  if (GmoPlanCommit((int *)&plan) == 0) {
    return NULL;
  }
  fresh = (GmoAttr *)GmoPlanTakeAttrs(n, &plan);
  for (i = 0; i < count; i++) {
    GmoAttrCopy(&fresh[i], &old[i], 1, &plan);
  }
  GmoPlanFree(&plan);
  GmoAttrArrayRelease((short *)old, count);
  rec = &fresh[count];
  m->attrCount = (u16)n;
  m->attrs = fresh;
  if (rec == NULL) {
    return rec;
  }
  flags = g_gmoAttrTypeFlags[type - 0x82];
  rec->flags = (rec->flags & 0xfe9ffffeu) | flags;
  rec->type = (u16)type;
  if (flags & 1) {
    rec->layerFlags14 |= 1;
    rec->layerFlags16 &= ~1;
  } else {
    rec->layerFlags14 &= ~1;
  }
  if (flags & 0x200000) {
    rec->blendDst = 1;
  } else {
    rec->blendDst = 7;
  }
  return rec;
}
