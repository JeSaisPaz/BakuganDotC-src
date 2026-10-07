// bdc 0x08823aa4 GfxEffectMgrCtor
#include "bdc.h"

/* Constructor of the 0xa0-byte effect manager (particle set, e.g. `"particle_00.ptb"`; a
   sprite layer (`GfxSpriteLayerCtor`) subclass with vtable `g_gfxEffectMgrVtbl` at `+0x74`): stores
   the particle data `data` (`+0x80`), relocates its definition table (`data + data[0]`, 0x20-byte
   entries whose first word, an offset, is turned into a pointer unless the first entry is already
   above 0x1000000) into `+0x88` with the count in `+0x84`, counts the texture names that follow the
   header (`+0x8c`), resolves them (`GfxEffectMgrResolveTextures`), marks the layer sorted, clears
   `g_gfxEffectAltMeshOnce`, the owns-data byte and the script flags, resets the update tick
   `g_gfxEffectTick` and enables lighting on the shared effect models 1..5
   (`g_gfxEffectModels`). In ad-hoc play with profile flag 0 set (`NetPlayHasManager`,
   `SaveGetProfileFlag0`) the sorted-sprite limit `maxSorted` is raised to 200. Returns `mgr`. */
void *GfxEffectMgrCtor(GfxEffectMgr *mgr, s32 *data)
{
    u8 *defs;
    u32 first;
    s32 i;
    const char *s;

    GfxSpriteLayerCtor(&mgr->base, 1);
    mgr->base.vtbl = g_gfxEffectMgrVtbl;
    mgr->data = data;
    defs = (u8 *)data + data[0];
    mgr->defs = defs;
    first = *(u32 *)defs;
    if (0x1000000 < first) {
        /* already relocated: the first entry holds a pointer */
        mgr->defCount = (s32)((first - (u32)data[0] - (u32)(uintptr_t)data) >> 5);
    } else {
        mgr->defCount = (s32)((first - (u32)data[0]) >> 5);
        for (i = 0; i < mgr->defCount; i++) {
            *(u32 *)(defs + i * 0x20) += (u32)(uintptr_t)data;
            data = mgr->data;
            defs = mgr->defs;
        }
    }

    s = (const char *)(data + 1);
    mgr->textureCount = 0;
    if ((const u8 *)s < defs) {
        while (*s != '\0') {
            s = &s[strlen(s) + 1]; /* next NUL-terminated name */
            mgr->textureCount = mgr->textureCount + 1;
            if (!((const u8 *)s < mgr->defs)) {
                break;
            }
        }
    }
    mgr->textures = NULL;
    GfxEffectMgrResolveTextures(mgr);
    mgr->base.sorted = 1;
    if (NetPlayHasManager() && SaveGetProfileFlag0()) {
        mgr->base.maxSorted = 200;
    }
    g_gfxEffectAltMeshOnce = 0;
    mgr->ownsData = 0;
    mgr->scriptFlags[2] = 0;
    mgr->scriptFlags[1] = 0;
    mgr->scriptFlags[0] = 0;
    g_gfxEffectTick = 0;
    g_gfxEffectModels[1]->lighting = 1;
    g_gfxEffectModels[2]->lighting = 1;
    g_gfxEffectModels[3]->lighting = 1;
    g_gfxEffectModels[4]->lighting = 1;
    g_gfxEffectModels[5]->lighting = 1;
    return mgr;
}
