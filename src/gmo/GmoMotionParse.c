// bdc 0x089da26c GmoMotionParse
#include "bdc.h"

/* Parses a motion chunk (`0xb`) into a motion info record (`GmoMotionInfoInit` layout): counts
   the `0xb3` sub-chunks (n), allocates from the motion arena `g_gmoMotionArena` (virtual method,
   vtable entry 7) a `0x10 * n` track array (each cleared with `GmoMotionTrackInit`) and a zeroed
   `2 * n` byte table and stores them in `motion->tracks` / `motion->table` with the count in
   `trackCount`, then walks the children: `0xb1` stores two floats in `startFrame`/`endFrame`,
   `0xb2` one float in `frameRate`, each `0xb3` fills the next track with `GmoMotionReadTrack`
   and `0xb4` stores its first argument word (truncated to u16) in `value0e`; other types are
   skipped. Allocations are not NULL-checked. */

void GmoMotionParse(const void *chunk, GmoMotionInfo *motion)
{
    const GmoChunk *c = (const GmoChunk *)chunk;
    const GmoChunk *end;
    const GmoChunk *p;
    const u32 *args;
    const VtblEntry *alloc;
    GmoMotionTrack *tracks;
    GmoMotionTrack *track;
    u16 *table;
    s32 n;
    s32 i;
    u32 type;

    n = GmoChunkCount(chunk, 0xb3);
    alloc = &((const VtblEntry *)g_gmoMotionArena->vtable)[7];
    tracks = ((GmoMotionTrack *(*)(void *, size_t))alloc->fn)((u8 *)g_gmoMotionArena + alloc->delta, n << 4);
    track = tracks;
    for (i = 0; i < n; i++) {
        GmoMotionTrackInit(track);
        track++;
    }
    alloc = &((const VtblEntry *)g_gmoMotionArena->vtable)[7];
    table = ((u16 *(*)(void *, size_t))alloc->fn)((u8 *)g_gmoMotionArena + alloc->delta, n * 2);
    memset(table, 0, n * 2);
    motion->tracks = PspAddr(tracks);
    motion->table = PspAddr(table);
    motion->trackCount = (u16)n;

    end = (const GmoChunk *)((const u8 *)c + c->size);
    if ((c->type & 0x8000) != 0) {
        p = end; /* short (bit-15) chunk: no children */
    } else {
        p = (const GmoChunk *)((const u8 *)c + c->childOffset);
    }
    track = tracks;
    while (p < end) {
        if ((p->type & 0x8000) != 0) {
            args = &p->childOffset; /* short (bit-15) chunk: 8-byte header */
        } else {
            args = (const u32 *)((const u8 *)p + p->headerSize);
        }
        type = p->type & 0x7fff;
        if (type == 0xb1) {
            motion->startFrame = ((const float *)args)[0];
            motion->endFrame = ((const float *)args)[1];
        } else if (type == 0xb2) {
            motion->frameRate = ((const float *)args)[0];
        } else if (type == 0xb3) {
            GmoMotionReadTrack(chunk, p, track);
            track++;
        } else if (type == 0xb4) {
            motion->value0e = (u16)args[0];
        }
        p = (const GmoChunk *)((const u8 *)p + p->size);
    }
}
