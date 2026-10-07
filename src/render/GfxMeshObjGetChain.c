// bdc 0x08a297f4 GfxMeshObjGetChain
#include "bdc.h"

/* Returns the effect chain embedded in a mesh object (`GfxMeshObjCtor`) (set up by
   `GfxEffectChainCtor`). */
GfxEffectChain *GfxMeshObjGetChain(GfxMeshObj *obj)
{
    return &obj->effectChain;
}
