// bdc 0x089e3938 CollisionCountMeshChunks
#include "bdc.h"

/* Counts the `"MESH"` chunks in a collision file's chunk list (`count` entries; each chunk is
   `{tag, size:24, …, pad:8 at +7}` and the next starts after the 4-aligned payload plus padding).
   Used by `CollisionMeshLoad` to size the part array. */

s32 CollisionCountMeshChunks(const void *chunks, s32 count)
{
  const CollisionFileChunk *chunk = (const CollisionFileChunk *)chunks;
  s32 found = 0;
  u32 i = 0;

  if (0 < count) {
    for (;;) {
      u32 pad = chunk->sizeAndPad >> 24;
      if (chunk->tag == g_meshChunkTag) {
        found++;
      }
      chunk = (const CollisionFileChunk *)(chunk->data + pad +
                                           (((chunk->sizeAndPad & 0xffffff) + 3) & ~3u));
      i = (i + 1) & 0xffff;
      if (count <= (s32)i) break;
    }
  }
  return found;
}
