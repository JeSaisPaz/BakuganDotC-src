// bdc 0x088702d8 BtlBakuganTexLoaderTaskUpdate
#include "bdc.h"

/* Update of the Bakugan texture loader task (BtlBakuganTexLoaderTask): step 1 starts loading the
   package g_btlBakuganTexPackPath (IoLzsPackageStartLoad, priority 10) and moves to step 2 once
   the request is pending; step 2 polls it (IoLzsPackagePoll). When loaded it counts the requested
   unit kinds (BtlCountKindsHasDuplicate into `kindCounts`) and, only if some kind occurs more than
   once, for each kind 1..20 occurring `n` > 1 times looks up the textures `<name>.001.tm2` …
   `<name>.(n-1).tm2` (g_btlBakuganTexNames, CorePackChainFindEntry); each one found is copied into
   a high-heap block (MemAlloc under MemLock with MemSetAllocFromLow`(true)`) and wrapped in a
   0x140-byte texture object (GfxTextureCtor) that owns the copy, appended to g_btlTexLoaderEntries
   (g_btlTexLoaderCount += 1, unbounded). Finally it deletes the package (virtual destructor, flags
   3) and removes itself (CoreTaskRemove). Other steps do nothing. */

void BtlBakuganTexLoaderTaskUpdate(CoreTask *task)
{
    BtlBakuganTexLoaderTask *self = (BtlBakuganTexLoaderTask *)task;
    char name[128];
    CorePackDirEntry *file;
    BtlTexLoaderEntry *entry;
    GfxTexture *tex;
    IoLzsPackage *pkg;
    const VtblEntry *dtor;
    void *data;
    bool fromLow;
    s32 kind;
    s32 i;

    if (self->step <= 0) {
        return;
    }
    if (self->step < 2) {
        if (IoLzsPackageStartLoad(self->package, g_btlBakuganTexPackPath, 10, 0, 0) == 0) {
            return;
        }
        self->step = 2;
    } else if (self->step >= 3) {
        return;
    }
    if (IoLzsPackagePoll(self->package, 0) == 0) {
        return;
    }

    if (BtlCountKindsHasDuplicate(self->kinds, self->kindCounts) != 0) {
        for (kind = 1; kind < 0x15; kind++) {
            if (self->kindCounts[kind] < 2) {
                continue;
            }
            for (i = 1; i < self->kindCounts[kind]; i++) {
                sprintf(name, "%s.%03d.tm2", g_btlBakuganTexNames[kind], i);
                file = (CorePackDirEntry *)CorePackChainFindEntry(self->package, name);
                if (file == NULL) {
                    continue;
                }
                entry = &g_btlTexLoaderEntries[g_btlTexLoaderCount];

                MemLock();
                fromLow = MemIsAllocFromLow();
                MemSetAllocFromLow(true);
                data = MemAlloc(file->size, NULL, 0);
                MemSetAllocFromLow(fromLow);
                MemUnlock();
                entry->data = data;
                memcpy(data, file->data, file->size);

                MemLock();
                fromLow = MemIsAllocFromLow();
                MemSetAllocFromLow(true);
                tex = (GfxTexture *)MemAlloc(sizeof(GfxTexture), NULL, 0);
                MemSetAllocFromLow(fromLow);
                MemUnlock();
                if (tex != NULL) {
                    GfxTextureCtor((CoreObject *)tex, name, entry->data, 1);
                }
                entry->texture = (CoreObject *)tex;
                /* written even when the allocation failed (tex == NULL), as in the binary */
                tex->ownsTim2 = 1;
                g_btlTexLoaderCount = g_btlTexLoaderCount + 1;
            }
        }
    }

    pkg = self->package;
    if (pkg != NULL) {
        dtor = &((const VtblEntry *)pkg->base.vtable)[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)pkg + dtor->delta, 3);
        self->package = NULL;
    }
    CoreTaskRemove(task, true);
}
