// bdc 0x08934e80 UiGauntletSetupAnimateFocus
#include "bdc.h"

/* Per-frame focus animation of the gauntlet setup screen (task 373, `maybe_UiScreen373Ctor`,
   class prefix `UiGauntletSetup`; card sprites `cc_card_L_%03d`, `c_set_OK_bo_*`, texts
   `DWCardName`/`DWCardHelp`; main update `UiGauntletSetupMainPhase`; focus area `+0x74`, item `+0x76`):
   advances `focusZoom` by 0.1 per frame while it is below 1 and scales the focused sprites by
   1 + 0.2 * focusZoom (capped at 1.2), placing them in front (posZ -200..-210). Focus area 0: sprite 18
   and the item's sprites `14+item`, `42+item`, `34+item` (these two also clear flags bit 5),
   `50+item`, `24+item`, `10+item`, `20+item`; otherwise sprites 39, 38 and 47. The other
   sprites are not touched. The sprite table is re-read from `base.data` on every access. */

void UiGauntletSetupAnimateFocus(UiGauntletSetup *self)
{
  GfxSprite **sprites;
  float zoom;
  float scale;

  zoom = self->focusZoom;
  sprites = (GfxSprite **)self->base.data;
  if (zoom < 1.0f) {
    zoom = zoom + 0.1f;
    self->focusZoom = zoom;
  }
  scale = zoom * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f))
    scale = 1.2f;

  if (self->focusArea == 0) {
    UiSpriteSetScaleRotation(sprites[18], scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[18]->posZ = -208.0f;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->item + 14], scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->item + 14]->posZ = -200.0f;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->item + 42], scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->item + 42]->posZ = -202.0f;
    ((GfxSprite **)self->base.data)[self->item + 42]->flags &= ~0x20u;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->item + 34], scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->item + 34]->posZ = -210.0f;
    ((GfxSprite **)self->base.data)[self->item + 34]->flags &= ~0x20u;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->item + 50], scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->item + 50]->posZ = -203.0f;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->item + 24], scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->item + 24]->posZ = -204.0f;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->item + 10], scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->item + 10]->posZ = -201.0f;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->item + 20], scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[self->item + 20]->posZ = -205.0f;
    return;
  }
  UiSpriteSetScaleRotation(sprites[39], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[39]->posZ = -202.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[38], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[38]->posZ = -200.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[47], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[47]->posZ = -201.0f;
}
