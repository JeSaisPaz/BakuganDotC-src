// bdc 0x0880a08c UiLanguageSelectStateBuild
#include "bdc.h"

/* State 1 of the language-selection screen: builds the layout's 0x21 sprites
   (`UiLayoutCreateSprites`), hides two of them, finds the `lan_pane` / `lan_pane_on` row textures
   (`GfxFindTexture`) and parks the screen in the empty state 3 (`UiLanguageSelectStateNop`)
   with sub-step 0. */

void UiLanguageSelectStateBuild(CoreTask *task)

{
  UiLanguageSelect *self = (UiLanguageSelect *)task;
  GfxSprite *sprite;

  UiLayoutCreateSprites(self->layer, self->sprites, 0x21);
  self->sprites[3]->flags &= ~1u;
  sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[11]);
  self->sprites[26] = sprite;
  sprite->blendMode = 3;
  sprite->flags &= ~1u;
  self->state = 3;
  self->subStep = 0;
  self->paneTex = GfxFindTexture("lan_pane");
  self->paneOnTex = GfxFindTexture("lan_pane_on");
}
