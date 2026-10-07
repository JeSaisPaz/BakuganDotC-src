// bdc 0x089f8fe8 GfxFabClipCtor
#include "bdc.h"

/* Constructor of a `.fab` clip (0x3c bytes, vtable `0x08af5884`): base node ctor (`list` is the caller's a1, passed straight through), clears the
   fab pointer, object list (`objHead..objCount`), `owner` and hold flag. Returns `clip`. */

void *GfxFabClipCtor(void *clip, void *list)
{
    GfxFabClip *c = (GfxFabClip *)clip;

    CoreObjectInitInList((CoreObject *)clip, list);
    c->vtable = &g_gfxFabClipVtable;
    c->fab = NULL;
    c->objTail = NULL;
    c->objHead = NULL;
    c->objCount = 0;
    c->holdAtEnd = 0;
    c->owner = NULL;
    return clip;
}
