// bdc 0x089d86e0 CoreObjectInit
#include "bdc.h"

/* Constructor of the `CoreObject` base class: sets the base vtable `g_coreObjectVtbl`, clears the
   owning `list`, links the object behind `chain` (`CoreObjectAppend`, `chain` may be NULL), clears the
   `unk08` and gives it the next unique id (`++``g_coreObjectCounter`). Returns `obj`. Used by
   about 25 constructors of derived engine objects, including the static constructors of the
   textures (`GfxFeedbackTextureStaticInit`). */
CoreObject *CoreObjectInit(CoreObject *obj, CoreObject *chain)
{
    obj->vtable = g_coreObjectVtbl;
    obj->list = NULL;
    CoreObjectAppend(obj, chain);
    obj->unk08 = 0;
    obj->id = ++g_coreObjectCounter;
    return obj;
}
