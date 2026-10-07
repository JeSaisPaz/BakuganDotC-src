// bdc 0x089f848c GfxFabDtor
#include "bdc.h"

/* Destructor of a `.fab` animation object (vtable `g_gfxFabVtable` slot 1): resets the vtable and,
   when loaded (`data` set), frees the definition array (`defs`), deletes the bitmap textures it
   created itself (`bitmapOwned[i]` set, virtual deleting dtor with flags 3), frees the bitmap and
   flag arrays (only when `bitmaps` is set), releases the clip list (`clips`,
   `CoreObjectListDeleteAll`), then runs the base dtor and frees the object when `flags & 1`.
   A NULL `fab` is ignored. */
void GfxFabDtor(GfxFab *fab, u32 flags)
{
    void *p;
    s32 i;

    if (fab == NULL)
        return;
    fab->vtable = &g_gfxFabVtable;
    if (fab->data != NULL) {
        p = fab->defs;
        MemLock();
        MemFree(p, NULL, 0);
        MemUnlock();
        if (fab->bitmaps != NULL) {
            for (i = 0; i < fab->bitmapCount; i++) {
                if (fab->bitmapOwned[i] != 0) {
                    CoreObject *bmp = (CoreObject *)fab->bitmaps[i];
                    if (bmp != NULL) {
                        /* virtual deleting destructor: vtable entry 1, flags 3 */
                        const VtblEntry *dtor = &((const VtblEntry *)bmp->vtable)[1];
                        ((void (*)(void *, s32))dtor->fn)((u8 *)bmp + dtor->delta, 3);
                    }
                }
            }
            p = fab->bitmaps;
            MemLock();
            MemFree(p, NULL, 0);
            MemUnlock();
            p = fab->bitmapOwned;
            MemLock();
            MemFree(p, NULL, 0);
            MemUnlock();
        }
        /* +0x88..+0x90 is an embedded CoreObjectList (head `clips`, tail, count) */
        CoreObjectListDeleteAll((CoreObjectList *)&fab->clips);
    }
    CoreObjectDtor((CoreObject *)fab, 0);
    if (flags & 1) {
        MemLock();
        MemFree(fab, NULL, 0);
        MemUnlock();
    }
}
