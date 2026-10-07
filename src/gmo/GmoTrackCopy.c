// bdc 0x08a1cb90 GmoTrackCopy
#include "bdc.h"

/* Copies a 0x10-byte motion track record (attribute `+0xd`, target `+0xe`, key data block `+4`):
   clears `dst` (`GmoMotionTrackDestroyContents`), copies the fields (`+2`, `+8..+0xf`) and the
   block pointer `+4`, taking a heap reference on it (`GmoHeapAddRef`) so the key data is shared.
   Returns `dst` (NULL when an argument is NULL). */

void *GmoTrackCopy(void *dst, const void *src, u32 flags, void *plan)
{
  GmoMotionTrack *d = (GmoMotionTrack *)dst;
  const GmoMotionTrack *s = (const GmoMotionTrack *)src;
  void *ptr;
  u16 ref, p8, pA;
  u8 pC, pD;

  if (((dst != NULL) && (src != NULL)) && (plan != NULL)) {
    if (dst != src) {
      GmoMotionTrackDestroyContents(d);
      ptr = s->data;
      ref = s->ref;
      p8 = s->param8;
      pA = s->paramA;
      pC = s->paramC;
      pD = s->paramD;
      d->kind = s->kind;
      d->param8 = p8;
      d->paramA = pA;
      d->paramC = pC;
      d->paramD = pD;
      d->ref = ref;
      d->data = ptr;
      GmoHeapAddRef(0, ptr);
    }
    return dst;
  }
  return NULL;
}
