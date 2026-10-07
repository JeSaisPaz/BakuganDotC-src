// bdc 0x088c7068 GameFieldScreenFxCtor
#include "bdc.h"

/* Constructor of the field screen-effect holder (field task `+0x610`, a pointer to a
   `GameFieldScreenFx` `{overlay sprite, sprite layer, effect manager}`): allocates the 0xc-byte
   holder from the low heap and stores it in `*fx` (NULL when the allocation fails); then a 2D sprite
   layer (0x80 bytes, `GfxSpriteLayerCtor`, `sorted = 1`), an effect manager for `"particle_02.ptb"`
   (0xa0 bytes, `GfxEffectMgrCtor`) and a full-screen 640×480 overlay sprite with tint (1, 0, 0),
   alpha 0.2, flag bit0 set, `blendMode = 1` and `geStencilTest = 0xdcff0004`. Returns `fx`. */

void **GameFieldScreenFxCtor(void **fx)
{
  bool fromLow;
  GameFieldScreenFx *sfx;
  GameFieldScreenFx *result;
  GfxSpriteLayer *layer;
  GfxSpriteLayer *layerResult;
  GfxEffectMgr *mgr;
  GfxEffectMgr *mgrResult;
  s32 *data;
  float pos[4] __attribute__((aligned(16)));

  result = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sfx = MemAlloc(sizeof(GameFieldScreenFx), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (sfx != NULL) {
    layerResult = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    layer = MemAlloc(sizeof(GfxSpriteLayer), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (layer != NULL) {
      GfxSpriteLayerCtor(layer, 0);
      layerResult = layer;
    }
    sfx->layer = layerResult;
    layerResult->sorted = 1;
    mgrResult = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mgr = MemAlloc(sizeof(GfxEffectMgr), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mgr != NULL) {
      data = CorePackChainFind(g_ioLzsPackages, "particle_02.ptb");
      GfxEffectMgrCtor(mgr, data);
      mgrResult = mgr;
    }
    sfx->effects = mgrResult;
    pos[2] = 0.0f;
    pos[1] = 0.0f;
    pos[0] = 0.0f;
    pos[3] = 0.0f;
    sfx->overlay = GfxSpriteLayerCreateSpriteByName(sfx->layer, "", pos, false);
    sfx->overlay->tint[0] = 1.0f;
    sfx->overlay->tint[1] = 0.0f;
    sfx->overlay->tint[2] = 0.0f;
    sfx->overlay->alpha = 0.2f;
    UiSpriteSetSize(640.0f, 480.0f, sfx->overlay);
    sfx->overlay->flags |= 1;
    sfx->overlay->blendMode = 1;
    sfx->overlay->geStencilTest = 0xdcff0004;
    result = sfx;
  }
  *fx = result;
  return fx;
}
