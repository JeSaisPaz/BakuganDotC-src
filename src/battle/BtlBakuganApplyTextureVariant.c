// bdc 0x088630e8 BtlBakuganApplyTextureVariant
#include "bdc.h"

/* Applies texture variant `variant` to a battle Bakugan's model by calling
   `GfxModelApplyTextureVariant` on its embedded model (`self->base`) and returns that call's
   result (non-zero when the last texture lookup succeeded; v0 is passed through).
   `BtlCreateBakugan` and `BtlMainSpawnBakugan` call it with the result of
   `BtlBakuganCountEarlierSameSpecies`, giving the second and later copies of a species different
   textures; both ignore the return value. */

int BtlBakuganApplyTextureVariant(BtlBakugan *self, int variant)
{
    return GfxModelApplyTextureVariant(&self->base, variant);
}
