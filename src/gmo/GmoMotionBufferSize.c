// bdc 0x089d948c GmoMotionBufferSize
#include "bdc.h"

/* Computes how many bytes of arena memory the GMO motion chunk `chunk` needs: `0x12` per `0xb3`
   track sub-chunk (`GmoChunkCount`) plus, for every `0xb3` child (`GmoMotionTrackDef`
   payload), the payload length of the `0xc` data chunk it references (`GmoChunkFind` with
   `dataRef & 0xfff`; `childOffset - dataOffset`, 0 for a short-form chunk) + 3 for alignment. A
   short-form `chunk` has no children. Returns 4 when the total is 0. */
s32 GmoMotionBufferSize(const void *chunk)
{
    const GmoChunk *base = chunk;
    const u8 *end = (const u8 *)base + base->size;
    const u8 *cur;
    s32 size = GmoChunkCount(chunk, 0xb3) * 0x12;

    if (base->type & 0x8000) {
        cur = end;
    } else {
        cur = (const u8 *)base + base->childOffset;
    }
    for (; cur < end; cur += ((const GmoChunk *)cur)->size) {
        const GmoChunk *child = (const GmoChunk *)cur;
        const GmoMotionTrackDef *def;
        const GmoChunk *data;
        s32 dataSize;

        if ((child->type & 0x7fff) != 0xb3) {
            continue;
        }
        if (child->type & 0x8000) {
            def = (const GmoMotionTrackDef *)(cur + __builtin_offsetof(GmoChunk, childOffset));
        } else {
            def = (const GmoMotionTrackDef *)(cur + child->headerSize);
        }
        data = GmoChunkFind(chunk, 0xc, def->dataRef & 0xfff);
        if (data->type & 0x8000) {
            dataSize = 0;
        } else {
            dataSize = (s32)data->childOffset - (s32)data->dataOffset;
        }
        size = dataSize + size + 3;
    }
    if (size == 0) {
        size = 4;
    }
    return size;
}
