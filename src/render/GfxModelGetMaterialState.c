// bdc 0x089e018c GfxModelGetMaterialState
#include "bdc.h"

/* Returns the state record (`+4` of the material, `GmoModelGetMaterial(data, index)`) of material `index`;
   flag bytes `+3`/`+4`, animation callback `+8`/`+0xc`. */

void *GfxModelGetMaterialState(GfxModel *self, s32 index)
{
    void **mat = GmoModelGetMaterial(self->data, index);
    return mat[1];
}
