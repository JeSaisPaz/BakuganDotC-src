// bdc 0x08a1cc44 GmoInstanceCopy
#include "bdc.h"

/* Copies a 0x20-byte mesh instance (`GmoInstance`); returns NULL if `dst`, `src` or `plan` is
   NULL, and `dst` untouched if `dst == src`. Otherwise clears `dst`
   (`GmoInstanceDestroyContents`) and copies every field except `refCount` and `next`; without a
   deep-copy flag (`flags & 0x81`) it only takes pool-1 references on the shared blocks; otherwise
   it carves (pool 2 if `flags` bit 31, else pool 1) and duplicates the 0x60-byte state block (if
   any), the vertex data (`vertexSize * vertexCount` bytes) and the display list
   (`displayListWords` words), relocating the list's address commands (`BASE`+`VADDR` pairs that
   hit the old vertex/state blocks, and every `0x09` jump, aimed at the list's last word), then
   flushes the data cache. Returns `dst`. */

void *GmoInstanceCopy(void *dst, const void *src, u32 flags, void *plan)
{
  GmoInstance *d = (GmoInstance *)dst;
  const GmoInstance *s = (const GmoInstance *)src;
  u32 *out;
  u32 *end;
  const u32 *in;
  uintptr_t last;
  u32 jumpCmd;
  u32 jumpBase;
  u32 word;
  u32 addr;
  s32 pool;
  u32 stateSize;
  u32 vertexBytes;
  u32 listBytes;

  if (dst == NULL || src == NULL || plan == NULL) {
    return NULL;
  }
  if (dst == src) {
    return dst;
  }
  if ((flags & 0x81) == 0) {
    flags = 0;
  }
  GmoInstanceDestroyContents(dst);
  d->pad02 = s->pad02;
  d->displayList = s->displayList;
  d->displayListWords = s->displayListWords;
  d->id = s->id;
  d->vertices = s->vertices;
  d->extra = s->extra;
  d->vertexSize = s->vertexSize;
  d->vertexCount = s->vertexCount;
  d->state = s->state;
  if (flags == 0) {
    GmoHeapAddRef(1, d->state);
    GmoHeapAddRef(1, d->displayList);
    GmoHeapAddRef(1, d->vertices);
    return dst;
  }

  vertexBytes = (u32)s->vertexSize * (u32)s->vertexCount;
  listBytes = (u32)s->displayListWords << 2;
  pool = ((s32)flags < 0) ? 2 : 1;
  stateSize = (s->state != NULL) ? 0x60 : 0;
  d->state = GmoPlanTake(plan, pool, 4, stateSize);
  d->displayList = (u32 *)GmoPlanTake(plan, pool, 4, listBytes);
  d->vertices = GmoPlanTake(plan, pool, 4, vertexBytes);
  memcpy(d->state, s->state, stateSize);
  memcpy(d->vertices, s->vertices, vertexBytes);

  out = d->displayList;
  end = out + d->displayListWords;
  last = (uintptr_t)(end - 1);
  jumpCmd = ((u32)last & 0xffffff) | 0x09000000;
  jumpBase = (((u32)last >> 24) << 16) | 0x10000000;
  in = s->displayList;
  for (; out < end; out++) {
    word = *in++;
    *out = word;
    if ((word >> 24) == 1) {
      /* VADDR: full address = BASE high byte (previous word) | low 24 bits */
      addr = ((out[-1] << 8) & 0xff000000) | (word & 0xffffff);
      if (addr == (u32)(uintptr_t)s->vertices) {
        addr = (u32)(uintptr_t)d->vertices;
      }
      if (addr == (u32)(uintptr_t)s->state) {
        addr = (u32)(uintptr_t)d->state;
      }
      *out = (addr & 0xffffff) | 0x01000000;
      out[-1] = ((addr >> 24) << 16) | 0x10000000;
    } else if ((word >> 24) == 9) {
      *out = jumpCmd;
      out[-1] = jumpBase;
    }
  }
  sceKernelDcacheWritebackAll();
  return dst;
}
