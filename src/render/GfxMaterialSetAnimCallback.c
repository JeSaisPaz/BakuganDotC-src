// bdc 0x089e0290 GfxMaterialSetAnimCallback
#include "bdc.h"

/* Stores an animation callback and its argument in `animCallback`/`animArg` of a material state record. */

void GfxMaterialSetAnimCallback(void *matState, void *callback, void *arg)

{
  GfxMaterialState *state = (GfxMaterialState *)matState;

  state->animCallback = callback;
  state->animArg = arg;
}
