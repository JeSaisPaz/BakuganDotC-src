// bdc 0x08905174 BtlDemoSceneLoad
#include "bdc.h"

/* Loads `.scb` battle demo script `index` (name from `g_btlDemoScbNames`) into a `BtlDemoScene`
   (`BtlDemoSceneCtor`): stores `index`, clears the key-track blocks (`track0`, `tracks[0..4]`),
   looks the file up in the pack chain of `g_ioLzsPackages` (`CorePackChainFind`,
   `CorePackChainFindSize`) and, when its size is non-zero, copies it into `buffer` (allocated
   from the low heap only if not already set), byte-swaps it in place (`BtlScbByteSwapBuffer`),
   points `header` at it, builds the object list and the event tables from the tables at byte
   offsets `header[1]` and `header[3]` (`BtlDemoSceneBuildObjects`,
   `BtlDemoSceneBuildEventTables`), clears the track blocks again and parses the key chunks at
   byte offset `header[4]` (size minus that offset) with `BtlDemoScbParse`. */

static void BtlDemoSceneClearTracks(BtlDemoScene *self)
{
    s32 i;

    memset(self->track0, 0, sizeof(self->track0));
    for (i = 0; i < 5; i++) {
        memset(self->tracks[i], 0, sizeof(self->tracks[i]));
    }
}

void BtlDemoSceneLoad(void *scene, s32 index)
{
    BtlDemoScene *self = (BtlDemoScene *)scene;
    void *src;
    u32 size;
    bool wasLow;
    u8 *data;
    u32 keyOffset;

    self->index = index;
    BtlDemoSceneClearTracks(self);
    src = CorePackChainFind(g_ioLzsPackages, g_btlDemoScbNames[index]);
    size = CorePackChainFindSize(g_ioLzsPackages, g_btlDemoScbNames[index]);
    if (size == 0) {
        return;
    }
    if (self->buffer == NULL) {
        MemLock();
        wasLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        self->buffer = MemAlloc(size, NULL, 0);
        MemSetAllocFromLow(wasLow);
        MemUnlock();
    }
    memcpy(self->buffer, src, size);
    data = (u8 *)self->buffer;
    BtlScbByteSwapBuffer((u32 *)data, (u32 *)data, size);
    self->header = (u32 *)data;
    BtlDemoSceneBuildObjects(&self->objects, (u32 *)(data + ((u32 *)data)[1]));
    BtlDemoSceneBuildEventTables(&self->events, (u32 *)(data + self->header[3]));
    keyOffset = self->header[4];
    BtlDemoSceneClearTracks(self);
    BtlDemoScbParse((u32 *)(data + keyOffset), self, size - keyOffset, 0);
}
