// bdc 0x088c7294 GameFieldScreenFxDtor
#include "bdc.h"

/* Destructor of the field screen-effect holder (`task+0x610`, pointer to `{GfxSprite *overlay,
   sprite layer, effect manager}`, built by `GameFieldScreenFxCtor`): deletes the sprite layer and
   the effect manager through their virtual destructors, frees the state and, when `flags & 1`, the
   holder. */

/* GCC 2.x virtual destructor call: slot 1 of the vtable, `this` adjusted by the entry delta. */
#define GF_VDELETE(obj, vtbl)                                                        \
  do {                                                                               \
    const VtblEntry *dtor_ = &((const VtblEntry *)(vtbl))[1];                        \
    ((void (*)(void *, u32))dtor_->fn)((u8 *)(obj) + dtor_->delta, 3);               \
  } while (0)

void GameFieldScreenFxDtor(void **fx, u32 flags)
{
  GameFieldScreenFx *state;
  GfxSpriteLayer *layer;
  GfxEffectMgr *effects;

  if (fx != NULL) {
    state = (GameFieldScreenFx *)*fx;
    if (state != NULL) {
      layer = state->layer;
      if (layer != NULL) {
        GF_VDELETE(layer, layer->vtbl);
      }
      effects = state->effects;
      if (effects != NULL) {
        GF_VDELETE(effects, effects->base.vtbl);
      }
      MemLock();
      MemFree(state, NULL, 0);
      MemUnlock();
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(fx, NULL, 0);
      MemUnlock();
    }
  }
}
