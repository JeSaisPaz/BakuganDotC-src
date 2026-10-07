// bdc 0x089bdddc IoLzsPackageRegister
#include "bdc.h"

/* Registers a loaded package's directory once (guarded by `registered`, +0x2a): relocates it with
   `IoPackDirRelocate` and, when `createTextures` is set and it holds TIM2 entries, allocates an
   array of that many 0x140-byte texture objects (`CxxVecNew` with ctor `GfxTextureCtorEmpty`, the
   heap placement temporarily set from `fromLow` under `MemLock`), stores it at `textures` (+0x38,
   NULL when the allocation failed; count `textureCount` +0x3c) and fills it with
   `IoPackDirInitTextures`. Then sets `registered`. */

void IoLzsPackageRegister(IoLzsPackage *self, u16 *dir, u8 createTextures, u8 fromLow)
{
    int count;
    bool wasLow;
    void *block;
    void *textures;
    int index;

    if (self->registered != 0) {
        return;
    }
    count = IoPackDirRelocate(dir);
    if (createTextures != 0 && count > 0) {
        self->textureCount = count;
        MemLock();
        wasLow = MemIsAllocFromLow();
        MemSetAllocFromLow(fromLow);
        block = MemAlloc(count * 0x140 + 0x10, NULL, 0);
        MemSetAllocFromLow(wasLow);
        MemUnlock();
        textures = NULL;
        if (block != NULL) {
            textures = CxxVecNew((u8 *)block + g_cxxVecCookieSize, count, 0x140,
                                 GfxTextureCtorEmpty, 0);
        }
        self->textures = textures;
        index = 0;
        IoPackDirInitTextures(dir, self, fromLow, &index);
    }
    self->registered = 1;
}
