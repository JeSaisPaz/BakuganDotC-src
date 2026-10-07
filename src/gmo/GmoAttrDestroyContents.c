// bdc 0x08a14918 GmoAttrDestroyContents
#include "bdc.h"

/* Releases what one record of a material's attribute array (0x40-byte records, type half-word at
   `+8`; array at `+8`, count `+0xc` of the owner) references: its child record (`+4`, recursive),
   the texture-layer record at `+0x24` (texture released through `GmoTextureRelease`) and the heap
   blocks `+0x28` and `+0xc`. Returns `attr` (NULL-safe). */

void *GmoAttrDestroyContents(void *attr)
{
    GmoAttr *a = (GmoAttr *)attr;
    GmoAttr *child;
    GmoLayer *layer;

    if (a == NULL) {
        return NULL;
    }
    child = a->child;
    if (child != NULL) {
        child->refCount--;
        if (child->refCount == 0) {
            GmoAttrDestroyContents(child);
            GmoHeapReleaseThunk(0, child);
        }
    }
    if (((a->layerRef + 1) & 0xffff0000) != 0) {
        layer = (GmoLayer *)(uintptr_t)a->layerRef;
        if (layer != NULL) {
            layer->f00--;
            if (layer->f00 == 0) {
                GmoTextureRelease((short *)layer->texture);
                GmoHeapReleaseThunk(0, layer);
            }
        }
    }
    GmoHeapReleaseThunk(0, a->data);
    GmoHeapReleaseThunk(0, a->block);
    return attr;
}
