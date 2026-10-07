// bdc 0x08a14a7c GmoMaterialArrayRelease
#include "bdc.h"

/* Drops one reference on each of `n` 0x10-byte material records (`refCount`); a record reaching 0
   releases its `info` chain (recursive, count 1), drops one reference on each of its `attrCount`
   attributes (one reaching 0 is emptied with `GmoAttrDestroyContents` and freed) and is freed
   itself (pool 0). Returns `arr` (NULL is ignored). */

short *GmoMaterialArrayRelease(short *arr, int n)
{
  GmoMaterial *mat;
  GmoAttr *attr;
  int count;
  int i;
  int j;

  if (arr != NULL) {
    mat = (GmoMaterial *)arr;
    for (i = 0; i < n; i++, mat++) {
      if (--mat->refCount != 0) {
        continue;
      }
      GmoMaterialArrayRelease((short *)mat->info, 1);
      attr = mat->attrs;
      count = mat->attrCount;
      if (attr != NULL) {
        for (j = 0; j < count; j++, attr++) {
          if (--attr->refCount == 0) {
            GmoAttrDestroyContents(attr);
            GmoHeapReleaseThunk(0, attr);
          }
        }
      }
      GmoHeapReleaseThunk(0, mat);
    }
  }
  return arr;
}
