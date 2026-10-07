// bdc 0x088c7374 GameFieldScreenFxUpdate
#include "bdc.h"

/* Per-frame update of the field screen-effect holder (`task+0x610`, pointer to `{GfxSprite
   *overlay, sprite layer, effect manager}`, built by `GameFieldScreenFxCtor`): updates the effect
   manager (`GfxEffectMgrUpdate`) and the sprite layer (`GfxSpriteLayerUpdateAll`). */

void GameFieldScreenFxUpdate(void **fx)

{
  GameFieldScreenFx *screenFx;

  screenFx = (GameFieldScreenFx *)*fx;
  GfxEffectMgrUpdate(screenFx->effects);
  GfxSpriteLayerUpdateAll(screenFx->layer);
  return;
}
