// bdc 0x0887061c BtlBakuganTexLoaderReleaseAll
#include "bdc.h"

/* Destroys every texture recorded by the Bakugan texture loader (the first `g_btlTexLoaderCount`
   entries of `g_btlTexLoaderEntries`) through its virtual deleting destructor (vtable entry 1,
   flags 3), clears each slot's texture pointer and resets the count to 0. The data copies are not
   freed here. Called by `BtlMainTaskDtor`. */
void BtlBakuganTexLoaderReleaseAll(void)
{
    BtlTexLoaderEntry *entry = g_btlTexLoaderEntries;
    s32 i;

    for (i = 0; i < g_btlTexLoaderCount; i++, entry++) {
        CoreObject *texture = entry->texture;

        if (texture != NULL) {
            const VtblEntry *dtor = &((const VtblEntry *)texture->vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)texture + dtor->delta, 3);
            entry->texture = NULL;
        }
    }
    g_btlTexLoaderCount = 0;
}
