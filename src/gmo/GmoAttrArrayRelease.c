// bdc 0x08a149ec GmoAttrArrayRelease
#include "bdc.h"

/* Drops one reference on each of `n` records of a material's attribute array (0x40-byte records,
   type half-word at `+8`; array at `+8`, count `+0xc` of the owner); records reaching 0 are emptied
   (`GmoAttrDestroyContents`) and freed. Returns `arr`. */

short *GmoAttrArrayRelease(short *arr, int n)
{
    GmoAttr *attr = (GmoAttr *)arr;
    int i;

    if (arr != NULL) {
        for (i = 0; i < n; i++, attr++) {
            attr->refCount--;
            if (attr->refCount == 0) {
                GmoAttrDestroyContents(attr);
                GmoHeapReleaseThunk(0, attr);
            }
        }
    }
    return arr;
}
