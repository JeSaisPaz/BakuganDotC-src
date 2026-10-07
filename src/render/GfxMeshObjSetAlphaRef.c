// bdc 0x08a297ec GfxMeshObjSetAlphaRef
#include "bdc.h"

/* Sets the alpha-test reference `alphaRef` of a mesh object (`GfxMeshObjCtor`) (0..255):
   `GfxMeshObjDrawList` emits it as `ATST` (`0xdb000000 | ref << 8 | 0xff0007`, pass when
   alpha >= ref). */

void GfxMeshObjSetAlphaRef(GfxMeshObj *obj, s32 ref)
{
    obj->alphaRef = ref;
}
