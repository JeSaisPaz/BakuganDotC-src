// bdc 0x08a1d340 GmoAttrCopy
#include "bdc.h"

/* Copies a 0x40-byte material attribute record (`GmoAttr`) from `src` into `dst`: returns NULL
   when `dst`, `src` or `plan` is NULL, and `dst` unchanged when `dst == src`. Otherwise clears
   `dst` (`GmoAttrDestroyContents`), copies the fields (not `refCount`, `_unk13`, `_unk18`), and
   either shares (`flags & 0x201` clear: `GmoHeapAddRef`) or duplicates through the plan the
   0x40-byte `block` and 0x10-byte `data` blocks; bumps the `child` record's reference count and
   passes the `layerRef` record (when it is an address, not an index/-1) to `GmoLayerArrayCopy`.
   Returns `dst`. */

void *GmoAttrCopy(void *dst, const void *src, u32 flags, void *plan)
{
    GmoAttr *d = (GmoAttr *)dst;
    const GmoAttr *s = (const GmoAttr *)src;
    void *block;
    size_t blockSize;
    size_t dataSize;
    void *layer;

    if (dst == NULL || src == NULL || plan == NULL) {
        return NULL;
    }
    if (dst == src) {
        return dst;
    }
    if ((flags & 0x201) == 0) {
        flags = 0;
    }
    GmoAttrDestroyContents(dst);
    d->blendOp = s->blendOp;
    d->blendSrc = s->blendSrc;
    block = s->block;
    d->flags02 = s->flags02;
    d->child = s->child;
    d->type = s->type;
    d->texMapMode = s->texMapMode;
    d->blendDst = s->blendDst;
    d->layerFlags14 = s->layerFlags14;
    d->layerFlags16 = s->layerFlags16;
    d->flags = s->flags;
    d->layerRef = s->layerRef;
    d->data = s->data;
    d->diffuse = s->diffuse;
    d->specular = s->specular;
    d->word34 = s->word34;
    d->block = block;
    d->emission = s->emission;
    d->word3c = s->word3c;
    if (flags == 0) {
        GmoHeapAddRef(0, block);
        GmoHeapAddRef(0, d->data);
    } else {
        blockSize = (s->block != NULL) ? 0x40 : 0;
        dataSize = (s->data != NULL) ? 0x10 : 0;
        d->block = GmoPlanTake(plan, 0, 0x40, blockSize);
        d->data = GmoPlanTake(plan, 0, 0x10, dataSize);
        memcpy(d->block, s->block, blockSize);
        memcpy(d->data, s->data, dataSize);
    }
    if (d->child != NULL) {
        d->child->refCount++;
    }
    /* layerRef values -1..0xfffe are indices / "none", not record addresses */
    layer = (void *)(uintptr_t)d->layerRef;
    if (((d->layerRef + 1) & 0xffff0000) == 0) {
        layer = NULL;
    }
    GmoLayerArrayCopy(layer, 1, 0, plan);
    return dst;
}
