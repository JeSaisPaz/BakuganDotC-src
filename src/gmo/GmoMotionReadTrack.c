// bdc 0x089d92a8 GmoMotionReadTrack
#include "bdc.h"

/* Fills one 0x10-byte track record from a `0xb3` sub-chunk of a motion chunk. The sub-chunk's
   argument words (at `+8` when type bit 15 is set, else after its header) hold the target ref
   (word 0, low 12 bits), the channel id (word 1) and the index of the data chunk `0xc` it uses
   (word 3, low 12 bits). Finds that chunk (`GmoChunkFind(gmo, 0xc, index)`, not NULL-checked),
   allocates its payload (`childOffset - dataOffset` bytes, or 0 for a bit-15 chunk) from the
   motion arena `g_gmoMotionArena` (virtual method, vtable entry 7), copies the payload there
   and stores the copy in `track->data`, ORs `GmoMotionTrackKind` into `track->kind`, and copies
   the data chunk's parameter words 0, 2 and 1 into `param8`, `paramA` and `paramC`, the channel id
   into `paramD` and the ref into `ref`. */

void GmoMotionReadTrack(const void *gmo, const void *trackChunk, GmoMotionTrack *track)
{
    const GmoChunk *tc = (const GmoChunk *)trackChunk;
    const GmoChunk *dc;
    const u32 *args;
    const u32 *params;
    const u8 *src;
    size_t n;
    const VtblEntry *alloc;
    void *dst;
    u32 kind;

    if ((tc->type & 0x8000) != 0) {
        args = &tc->childOffset; /* short (bit-15) chunk: 8-byte header */
    } else {
        args = (const u32 *)((const u8 *)tc + tc->headerSize);
    }
    dc = (const GmoChunk *)GmoChunkFind(gmo, 0xc, args[3] & 0xfff);
    if ((dc->type & 0x8000) != 0) {
        params = &dc->childOffset; /* short (bit-15) chunk: 8-byte header */
        src = (const u8 *)&dc->childOffset;
        n = 0;
    } else {
        params = (const u32 *)((const u8 *)dc + dc->headerSize);
        src = (const u8 *)dc + dc->dataOffset;
        n = dc->childOffset - dc->dataOffset;
    }
    alloc = &((const VtblEntry *)g_gmoMotionArena->vtable)[7];
    dst = ((void *(*)(void *, size_t))alloc->fn)((u8 *)g_gmoMotionArena + alloc->delta, n);
    memcpy(dst, src, n);
    kind = GmoMotionTrackKind(args[1], params[0]);
    track->data = dst;
    track->kind = track->kind | (u16)kind;
    track->param8 = (u16)params[0];
    track->paramA = (u16)params[2];
    track->paramC = (u8)params[1];
    track->paramD = (u8)args[1];
    track->ref = (u16)args[0] & 0xfff;
}
