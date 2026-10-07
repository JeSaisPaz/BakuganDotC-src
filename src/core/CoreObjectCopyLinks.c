// bdc 0x089d8a40 CoreObjectCopyLinks
#include "bdc.h"

/* Copies the first five header words of a `CoreObject` (`prev`, `next`, `unk08`, `id`,
   `list`) from `src` to `dst`, leaving the vtable alone. `GfxSpriteCopy` uses it to restore a
   sprite's list links after a bulk field copy. */
void CoreObjectCopyLinks(CoreObject *src, CoreObject *dst)
{
    dst->prev = src->prev;
    dst->next = src->next;
    dst->unk08 = src->unk08;
    dst->id = src->id;
    dst->list = src->list;
}
