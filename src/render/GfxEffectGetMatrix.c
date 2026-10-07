// bdc 0x08a297dc GfxEffectGetMatrix
#include "bdc.h"

/* Returns the address of the effect's local 4x4 matrix. */
float *GfxEffectGetMatrix(GfxEffect *effect)
{
    return effect->matrix;
}
