// bdc 0x08904988 BtlStageCamSceneLoad
#include "bdc.h"

/* Loads stage camera script `index` (`g_btlStageCamFileNames`) into a `BtlStageCamScene`:
   clears the key-array blocks, looks the file up in `g_ioLzsPackages` (`CorePackChainFind`,
   `CorePackChainFindSize`) and, for a non-zero size, copies it into `fileBuf` (allocated from
   the low heap on first use only), byte-swaps it in place (`BtlScbByteSwapBuffer`), clears the
   blocks again and parses it as a raw chunk list (`BtlDemoScbParse`, `raw = 1`). */
void BtlStageCamSceneLoad(BtlStageCamScene *scene, int index)
{
    void *src;
    u32 size;
    u32 *buf;
    bool wasLow;

    memset(scene->head, 0, sizeof(scene->head));
    memset(scene->blocks[0], 0, sizeof(scene->blocks[0]));
    memset(scene->blocks[1], 0, sizeof(scene->blocks[1]));
    memset(scene->blocks[2], 0, sizeof(scene->blocks[2]));
    memset(scene->blocks[3], 0, sizeof(scene->blocks[3]));
    memset(scene->blocks[4], 0, sizeof(scene->blocks[4]));
    src = CorePackChainFind(g_ioLzsPackages, g_btlStageCamFileNames[index]);
    size = CorePackChainFindSize(g_ioLzsPackages, g_btlStageCamFileNames[index]);
    if (size == 0) {
        return;
    }
    if (scene->fileBuf == NULL) {
        MemLock();
        wasLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        scene->fileBuf = MemAlloc(size, NULL, 0);
        MemSetAllocFromLow(wasLow);
        MemUnlock();
    }
    memcpy(scene->fileBuf, src, size);
    buf = scene->fileBuf;
    BtlScbByteSwapBuffer(buf, buf, size);
    memset(scene->head, 0, sizeof(scene->head));
    memset(scene->blocks[0], 0, sizeof(scene->blocks[0]));
    memset(scene->blocks[1], 0, sizeof(scene->blocks[1]));
    memset(scene->blocks[2], 0, sizeof(scene->blocks[2]));
    memset(scene->blocks[3], 0, sizeof(scene->blocks[3]));
    memset(scene->blocks[4], 0, sizeof(scene->blocks[4]));
    BtlDemoScbParse(buf, scene, size, 1);
}
