// bdc 0x089f8b40 GfxFabLoad
#include "bdc.h"

#define FAB_TAG_DEFI 0x49464544u /* "DEFI" */
#define FAB_TAG_MCLP 0x504c434du /* "MCLP" */
#define FAB_TAG_BITM 0x4d544942u /* "BITM" */
#define FAB_TAG_FLAB 0x42414c46u /* "FLAB" */

/* Parses a loaded `.fab` 2D animation file (`GfxFabFile` at `data`, `GfxFabChunk` list):
   counts the `DEFI` definitions into `defCount` and allocates `defs`, then in one pass stores each
   `DEFI` payload into `defs`, builds one 0x3c-byte `GfxFabClip` per `MCLP` chunk
   (`GfxFabClipCtor` + `GfxFabClipBind`; the first becomes `rootClip` with `holdAtEnd` set),
   counts `BITM` bitmaps (`bitmapCount`) and `FLAB` labels (first at `labels`, count `labelCount`).
   Extends the root clip's `lastFrame` to the `"StageMaxFrame"` label value (`GfxFabFindLabel`)
   if larger. If there are bitmaps, allocates `bitmaps`/`bitmapOwned` and resolves each `BITM` to
   the texture `"fab_%s%02d"` (file base name + bitmap index) via `GfxTryFindTexture`, else
   creates it from the chunk's TIM2 data (`GfxTextureCtor`, marked owned), with
   `g_gfxTexVramEnabled` cleared during the pass and restored after. */
void GfxFabLoad(GfxFab *fab)
{
    GfxFabFile *file = (GfxFabFile *)fab->data;
    s32 chunkCount = file->chunkCount;
    GfxFabChunk *chunk = file->chunks;
    u32 defCount = fab->defCount;
    char *maxFrameLabel = "StageMaxFrame";
    char baseName[256];
    char texName[256];
    bool fromLow;
    s32 i;

    for (i = 0; i < chunkCount; i++) {
        if (chunk->tag == FAB_TAG_DEFI) {
            fab->defCount = defCount + 1;
            defCount = fab->defCount;
        }
        chunk = (GfxFabChunk *)((u8 *)chunk + chunk->size);
    }

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    u16 **defs = MemAlloc(defCount << 2, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    fab->defs = defs;

    chunk = ((GfxFabFile *)fab->data)->chunks;
    s32 defIndex = 0;
    for (i = 0; i < chunkCount; i++) {
        u32 tag = chunk->tag;
        if (tag == FAB_TAG_MCLP) {
            GfxFabClip *clip;
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            clip = MemAlloc(0x3c, NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            if (clip != NULL) {
                GfxFabClipCtor(clip, &fab->clips);
            }
            GfxFabClipBind(clip, fab, (GfxFabClipDef *)chunk);
            if (fab->rootClip == NULL) {
                fab->rootClip = clip;
                clip->holdAtEnd = 1;
            }
        } else if (tag == FAB_TAG_BITM) {
            fab->bitmapCount++;
        } else if (tag == FAB_TAG_DEFI) {
            fab->defs[defIndex] = (u16 *)(chunk + 1);
            defIndex++;
        } else if (tag == FAB_TAG_FLAB) {
            if (fab->labels == NULL) {
                fab->labels = (GfxFabLabel *)chunk;
            }
            fab->labelCount++;
        }
        chunk = (GfxFabChunk *)((u8 *)chunk + chunk->size);
    }

    u16 lastFrame = fab->rootClip->def->lastFrame;
    if ((s32)lastFrame < (s32)GfxFabFindLabel(fab, maxFrameLabel)) {
        u16 maxFrame = GfxFabFindLabel(fab, maxFrameLabel);
        fab->rootClip->def->lastFrame = maxFrame;
    }

    u32 bitmapCount = fab->bitmapCount;
    if (bitmapCount == 0) {
        return;
    }

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    void **bitmaps = MemAlloc(bitmapCount << 2, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    fab->bitmaps = bitmaps;

    bitmapCount = fab->bitmapCount;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    u8 *owned = MemAlloc(bitmapCount, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    chunk = ((GfxFabFile *)fab->data)->chunks;
    fab->bitmapOwned = owned;

    strcpy(baseName, fab->name);
    char *dot = strrchr(baseName, '.');
    if (dot != NULL) {
        *dot = '\0';
    }

    u8 savedVram = g_gfxTexVramEnabled;
    g_gfxTexVramEnabled = 0;
    s32 bitmapIndex = 0;
    for (i = 0; i < chunkCount; i++) {
        if (chunk->tag == FAB_TAG_BITM) {
            sprintf(texName, "fab_%s%02d", baseName, bitmapIndex);
            void *tim2 = (void *)(((uintptr_t)(chunk + 1) + 0xf) & ~(uintptr_t)0xf);
            fab->bitmaps[bitmapIndex] = GfxTryFindTexture(texName);
            if (fab->bitmaps[bitmapIndex] == NULL) {
                CoreObject *tex;
                MemLock();
                fromLow = MemIsAllocFromLow();
                MemSetAllocFromLow(true);
                tex = MemAlloc(0x140, NULL, 0);
                MemSetAllocFromLow(fromLow);
                MemUnlock();
                if (tex != NULL) {
                    GfxTextureCtor(tex, texName, tim2, 1);
                }
                fab->bitmaps[bitmapIndex] = tex;
                fab->bitmapOwned[bitmapIndex] = 1;
            } else {
                fab->bitmapOwned[bitmapIndex] = 0;
            }
            bitmapIndex++;
        }
        chunk = (GfxFabChunk *)((u8 *)chunk + chunk->size);
    }
    g_gfxTexVramEnabled = savedVram;
}
