// bdc 0x08a141c0 GmoAttrCtor
#include "bdc.h"

/* In-place constructor of the 0x40-byte material attribute records carved by `GmoPlanTakeAttrs`:
   reference count `+0x0` = 1, `+0x8` = 0x82, bytes `+0x11` = 6 and `+0x12` = 7, `+0x20` =
   `0x00100000`, `+0x24` = -1, everything else cleared. Returns `rec` (NULL-safe). */

void *GmoAttrCtor(void *rec)
{
    GmoAttr *a = (GmoAttr *)rec;

    if (a != NULL) {
        a->refCount = 1;
        a->type = 0x82;
        a->blendSrc = 6;
        a->blendDst = 7;
        a->flags = 0x100000;
        a->layerRef = 0xffffffff;
        a->flags02 = 0;
        a->child = NULL;
        a->texMapMode = 0;
        a->block = NULL;
        a->blendOp = 0;
        a->layerFlags14 = 0;
        a->layerFlags16 = 0;
        a->data = NULL;
        a->diffuse = 0;
        a->specular = 0;
        a->word34 = 0;
        a->emission = 0;
        a->word3c = 0;
    }
    return rec;
}
