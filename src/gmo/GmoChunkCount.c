// bdc 0x08a198cc GmoChunkCount
#include "bdc.h"

/* Counts the child chunks of a GMO chunk. `chunk` is a `{u16 type (bit 15 = short header form), u16
   header length, u32 total length at +4, u32 offset of the first child at +8, ...}` record; its
   children run from `chunk + childOffset` (the short form has no children) to `chunk +
   totalLength`, each child advancing by its own `total length` field at `+4`. With `type == 0`
   every child is counted, otherwise only those whose type (bit 15 masked off) equals `type`. A NULL
   chunk yields 0. */

s32 GmoChunkCount(const void *chunk, u32 type)
{
    const GmoChunk *c = (const GmoChunk *)chunk;
    const GmoChunk *end;
    const GmoChunk *p;
    s32 n;

    if (c == NULL) {
        return 0;
    }
    end = (const GmoChunk *)((const u8 *)c + c->size);
    p = end;
    if ((s16)c->type >= 0) {
        p = (const GmoChunk *)((const u8 *)c + c->childOffset);
    }
    n = 0;
    if (p < end) {
        if (type == 0) {
            do {
                p = (const GmoChunk *)((const u8 *)p + p->size);
                n++;
            } while (p < end);
            return n;
        }
        while (1) {
            if (type == (p->type & 0x7fff)) {
                n++;
            }
            p = (const GmoChunk *)((const u8 *)p + p->size);
            if (p >= end) {
                break;
            }
        }
        return n;
    }
    return 0;
}
