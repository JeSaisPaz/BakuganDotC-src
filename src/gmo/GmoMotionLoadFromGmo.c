// bdc 0x089d9f60 GmoMotionLoadFromGmo
#include "bdc.h"

/* Registers every motion chunk (`0xb`) of a loaded GMO file: first counts the chunks whose name (at
   chunk `+0x10`) is not yet registered (`GmoMotionContains`) and sums their
   `GmoMotionBufferSize`; if there are any and the sum is non-zero, allocates an array of `n`
   0x84-byte `GmoMotionEntry` elements (`CxxVecNew`, constructor `GmoMotionEntryCtor`, low heap)
   stored in `g_gmoMotionArena`, reserves an arena of the summed size on the first element
   (virtual, vtable entry 6), then for each new chunk links the entry into `g_gmoMotionRegistry`,
   names it (virtual, vtable entry 3), marks it as an array element (`arrayOwner = 1`), stores its
   index (`GmoMotionIndexBase`), initialises the info record returned by vtable entry 2
   (`GmoMotionInfoInit`) and fills it with `GmoMotionParse`. The last element's `arrayOwner` is
   set to the array base so `GmoMotionFree` can release the block. A failed allocation is not
   checked (the NULL array is dereferenced). */

void GmoMotionLoadFromGmo(const void *gmo)
{
    const GmoChunk *root = (const GmoChunk *)gmo;
    const u8 *end;
    const u8 *cur;
    s32 count = 0;
    s32 arenaSize = 0;
    s32 index = 0;
    bool fromLow;
    void *block;
    GmoMotionEntry *entries;
    GmoMotionEntry *entry;
    const VtblEntry *vt;

    end = (const u8 *)root + root->size;
    if (root->type & 0x8000) {
        cur = end;
    } else {
        cur = (const u8 *)root + root->childOffset;
    }
    for (; cur < end; cur += ((const GmoChunk *)cur)->size) {
        if ((((const GmoChunk *)cur)->type & 0x7fff) != 0xb) {
            continue;
        }
        if (GmoMotionContains(GmoMotionMgrGet(), (const char *)(cur + sizeof(GmoChunk)))) {
            continue;
        }
        arenaSize += GmoMotionBufferSize(cur);
        count++;
    }
    if (count <= 0 || arenaSize == 0) {
        return;
    }

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(count * (s32)sizeof(GmoMotionEntry) + 0x10, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    entries = NULL;
    if (block != NULL) {
        entries = (GmoMotionEntry *)CxxVecNew((u8 *)block + g_cxxVecCookieSize, count,
                                              sizeof(GmoMotionEntry), (void *)GmoMotionEntryCtor, 0);
    }
    g_gmoMotionArena = (CoreNode *)entries;
    vt = (const VtblEntry *)((CoreNode *)entries)->vtable + 6;
    ((void (*)(void *, s32))vt->fn)((u8 *)entries + vt->delta, arenaSize);

    end = (const u8 *)root + root->size;
    if (root->type & 0x8000) {
        cur = end;
    } else {
        cur = (const u8 *)root + root->childOffset;
    }
    entry = entries;
    for (; cur < end; cur += ((const GmoChunk *)cur)->size) {
        const char *name = (const char *)(cur + sizeof(GmoChunk));
        CoreNode *node = (CoreNode *)entry;
        GmoMotionInfo *info;

        if ((((const GmoChunk *)cur)->type & 0x7fff) != 0xb) {
            continue;
        }
        if (GmoMotionContains(GmoMotionMgrGet(), name)) {
            continue;
        }
        CoreNodeLink(node, g_gmoMotionRegistry, 0);
        vt = (const VtblEntry *)node->vtable + 3;
        ((void (*)(void *, const char *))vt->fn)((u8 *)entry + vt->delta, name);
        entry->arrayOwner = (void *)(uintptr_t)1; /* sentinel: element of a CxxVecNew array */
        node->unk08 = GmoMotionIndexBase(index);
        vt = (const VtblEntry *)node->vtable + 2;
        info = ((GmoMotionInfo *(*)(void *))vt->fn)((u8 *)entry + vt->delta);
        GmoMotionInfoInit(info);
        GmoMotionParse(cur, info);
        index++;
        entry++;
    }
    entries[count - 1].arrayOwner = entries;
}
