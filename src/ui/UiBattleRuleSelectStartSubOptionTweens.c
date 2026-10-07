// bdc 0x08953960 UiBattleRuleSelectStartSubOptionTweens
#include "bdc.h"

/* Starts the pop tweens (`UiTweenBegin`, 1.4, mode 3) of the three sub-option buttons of
   `UiBattleRuleSelect` (sprites 0x0b–0x0d, or 0x0e–0x10 when
   `SaveGetProfileFlag0` is set); disabled options (`subEnabled[i]` clear) are greyed
   (tint 0.5, alpha 0). Each sprite gets layer mask 2 and is made visible. */

void UiBattleRuleSelectStartSubOptionTweens(UiBattleRuleSelect *self, u8 closing)
{
  u32 first;
  u32 last;
  u32 id;
  int i;
  GfxSprite *sprite;

  if (SaveGetProfileFlag0() == 0) {
    first = 0xb;
    last = 0xd;
  } else {
    first = 0xe;
    last = 0x10;
  }
  for (id = first, i = 0; id <= last; id++, i++) {
    sprite = ((GfxSprite **)self->base.data)[id];
    if (self->subEnabled[i] == 0) {
      sprite->tint[0] = 0.5f;
      sprite->tint[1] = 0.5f;
      sprite->tint[2] = 0.5f;
      sprite->alpha = 0.0f;
      sprite = ((GfxSprite **)self->base.data)[id];
    }
    sprite->layerMask = 2;
    sprite = ((GfxSprite **)self->base.data)[id];
    sprite->flags |= 1;
    UiTweenBegin(1.4f, closing, ((GfxSprite **)self->base.data)[id], &self->tweens[id], 3);
  }
}
