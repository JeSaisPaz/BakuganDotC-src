// bdc 0x08a297e4 GfxEffectSetAlpha
#include "bdc.h"

/* Stores the 0..255 alpha of an effect (`+0xe8`). */
void GfxEffectSetAlpha(GfxEffect *effect, s32 alpha)
{
    effect->alpha = alpha;
}
