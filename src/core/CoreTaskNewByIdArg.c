// bdc 0x088123bc CoreTaskNewByIdArg
#include "bdc.h"

/* Inlined in every case: allocates `size` bytes from the low end of the game heap (the placement
   policy is saved and restored around `MemAlloc`, all under `MemLock`). */
static inline void *CoreTaskAllocLow(s32 size)
{
    bool fromLow;
    void *mem;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    return mem;
}

/* Second task factory, behind `CoreTaskCreateDefault`: like `CoreTaskNewById` but for classes
   whose constructor takes one extra argument (`arg`, forwarded as a byte for some classes). Returns
   the constructed task for ten known ids, otherwise NULL (no default object); NULL too when the
   allocation fails. `arg` is the unit pointer for the cut-in task (481) and an integer carried in
   the pointer for the others: passed whole to 106/198/520, as an unsigned byte to 374/375/392 and
   as a signed byte (`category`) to the collection screens 312-314. */
CoreTask *CoreTaskNewByIdArg(s32 id, void *arg)
{
    void *mem;

    switch (id) {
    case 106:
        mem = CoreTaskAllocLow(sizeof(GameStoryMovie));
        if (mem == NULL) {
            return NULL;
        }
        GameFieldMovieTaskCtor(mem, (u32)(uintptr_t)arg);
        return mem;
    case 198:
        mem = CoreTaskAllocLow(sizeof(UiCopyrightTask));
        if (mem == NULL) {
            return NULL;
        }
        UiCopyrightTaskCtorArg(mem, (u32)(uintptr_t)arg);
        return mem;
    case 312:
        mem = CoreTaskAllocLow(sizeof(UiCollectionSphere));
        if (mem == NULL) {
            return NULL;
        }
        UiCollectionSphereCtor(mem, (s8)(uintptr_t)arg);
        return mem;
    case 313:
        mem = CoreTaskAllocLow(sizeof(UiCollectionCard));
        if (mem == NULL) {
            return NULL;
        }
        UiCollectionCardCtor(mem, (s8)(uintptr_t)arg);
        return mem;
    case 314:
        mem = CoreTaskAllocLow(sizeof(UiCollectionFigure));
        if (mem == NULL) {
            return NULL;
        }
        UiCollectionFigureCtor(mem, (s8)(uintptr_t)arg);
        return mem;
    case 374:
        mem = CoreTaskAllocLow(sizeof(UiPasscode));
        if (mem == NULL) {
            return NULL;
        }
        UiPasscodeCtor(mem, (u8)(uintptr_t)arg);
        return mem;
    case 375:
        mem = CoreTaskAllocLow(sizeof(UiUnlockResult));
        if (mem == NULL) {
            return NULL;
        }
        UiUnlockResultCtor(mem, (u8)(uintptr_t)arg);
        return mem;
    case 392:
        mem = CoreTaskAllocLow(sizeof(UiHologramView));
        if (mem == NULL) {
            return NULL;
        }
        UiHologramViewCtor(mem, (u8)(uintptr_t)arg);
        return mem;
    case 481:
        mem = CoreTaskAllocLow(sizeof(BtlCutInTask));
        if (mem == NULL) {
            return NULL;
        }
        BtlCutInTaskCtor(mem, arg);
        return mem;
    case 520:
        mem = CoreTaskAllocLow(sizeof(GameStoryMovie));
        if (mem == NULL) {
            return NULL;
        }
        GameStoryMovieCtor(mem, (u32)(uintptr_t)arg);
        return mem;
    default:
        return NULL;
    }
}
