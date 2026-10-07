// bdc 0x08a1ca48 GmoLayerCopy
#include "bdc.h"

/* Copies a 0x10-byte texture-layer record: clears `dst` (`GmoLayerDestroyContents`), copies its
   flags (`+2`) and texture (`+4`), and takes a texture reference with `GmoTextureShare`,
   converting the copy flags (base 2, 0x10000 → |8, 0x20000/0x40000/0x80000 → |0x10/0x20/0x40) when
   a deep copy is requested (`flags & 0xf0401`, or flag 2 with a dynamic texture,
   `GmoTextureIsDynamic`); otherwise shares with flags 0. Returns `dst` (unchanged when
   `dst == src`), or NULL when `dst`, `src` or `plan` is NULL. */

void *GmoLayerCopy(void *dst, const void *src, u32 flags, void *plan)
{
    GmoLayer *d = dst;
    const GmoLayer *s = src;
    u32 shareFlags;

    if (d == NULL || s == NULL || plan == NULL) {
        return NULL;
    }
    if (d == s) {
        return d;
    }
    if ((flags & 0xf0401) == 0 &&
        ((flags & 2) == 0 || GmoTextureIsDynamic(s->texture) == 0)) {
        flags = 0;
    }
    GmoLayerDestroyContents(d);
    d->flags = s->flags;
    d->texture = s->texture;
    shareFlags = 0;
    if (flags != 0) {
        shareFlags = (flags & 0x10000) ? 10 : 2;
        if (flags & 0x20000) {
            shareFlags |= 0x10;
        }
        if (flags & 0x40000) {
            shareFlags |= 0x20;
        }
        if (flags & 0x80000) {
            shareFlags |= 0x40;
        }
    }
    d->texture = GmoTextureShare(s->texture, shareFlags);
    return d;
}
