// bdc 0x088c7558 GameFieldScreenFxGetEffects
#include "bdc.h"

/* Returns the effect manager of the field screen-effect holder (`task+0x610`, pointer to
   `{GfxSprite *overlay, sprite layer, effect manager}`, built by `GameFieldScreenFxCtor`). */

void *GameFieldScreenFxGetEffects(void **fx)
{
  return ((GameFieldScreenFx *)*fx)->effects;
}
