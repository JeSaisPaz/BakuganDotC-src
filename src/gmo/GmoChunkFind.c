// bdc 0x08a1996c GmoChunkFind
#include "bdc.h"

/* Returns the `index`-th (0-based) child of the GMO chunk `chunk` whose type (bit 15 masked) equals
   `type`, or NULL when there are fewer or `chunk` is NULL. `type == 0` matches every child. Chunk
   layout as in `GmoChunkCount`. */

const void *GmoChunkFind(const void *chunk, u32 type, s32 index)
{
    const GmoChunk *c = (const GmoChunk *)chunk;
    const GmoChunk *end;
    const GmoChunk *p;

    if (c == NULL) {
        return NULL;
    }
    end = (const GmoChunk *)((const u8 *)c + c->size);
    p = end;
    if ((s16)c->type >= 0) {
        p = (const GmoChunk *)((const u8 *)c + c->childOffset);
    }
    if (p < end) {
        if (type == 0) {
            do {
                if (--index == -1) {
                    return p;
                }
                p = (const GmoChunk *)((const u8 *)p + p->size);
            } while (p < end);
            return NULL;
        }
        while (1) {
            if (type == (p->type & 0x7fff)) {
                if (--index == -1) {
                    return p;
                }
            }
            p = (const GmoChunk *)((const u8 *)p + p->size);
            if (p >= end) {
                break;
            }
        }
    }
    return NULL;
}
