// bdc 0x089f8958 GfxFabFindDef
#include "bdc.h"

/* Returns the `DEFI` definition record whose first u16 (id) equals `id`, searching the fab's
   definition array (`defs`, count `defCount`), or NULL. */

u16 *GfxFabFindDef(GfxFab *fab, u32 id)
{
    int i;
    u16 **p;

    i = 0;
    if (fab->defCount != 0) {
        p = fab->defs;
        do {
            i++;
            if (**p == id) {
                return *p;
            }
            p++;
        } while (i < (int)fab->defCount);
    }
    return NULL;
}
