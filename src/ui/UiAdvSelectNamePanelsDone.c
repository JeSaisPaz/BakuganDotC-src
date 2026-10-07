// bdc 0x0891b12c UiAdvSelectNamePanelsDone
#include "bdc.h"

/* While `namePanelsDir` is set, advances the two name-panel tweens started by
   `UiAdvSelectTweenNamePanels` (sprites 28 and 29, tweens 28/29, `UiTweenUpdate` 1.2 -> 1.0
   over 16 frames, `namePanelsHide` as fade-out) and copies each panel's alpha to its companion
   sprites 32+i and 30+i. When showing (mode 7), the companions also follow the panel: position =
   panel position + `namePanelOffset` * panel scale, and the panel's scale. Clears `namePanelsDir`
   once any of the tweens reports completion. */

void UiAdvSelectNamePanelsDone(UiAdvSelect *self)
{
  u8 finished = 0;
  s32 i;

  if (self->namePanelsDir == 0) {
    return;
  }
  if (self->namePanelsHide == 0) {
    for (i = 0; i < 2; i++) {
      bool done = UiTweenUpdate(1.2f, 1.0f, 16.0f, self->namePanelsHide,
                                ((GfxSprite **)self->base.data)[28 + i], &self->tweens[28 + i], 7);

      ((GfxSprite **)self->base.data)[32 + i]->alpha = ((GfxSprite **)self->base.data)[28 + i]->alpha;
      ((GfxSprite **)self->base.data)[32 + i]->posX =
          ((GfxSprite **)self->base.data)[28 + i]->posX +
          self->namePanelOffset[i * 4 + 0] * ((GfxSprite **)self->base.data)[28 + i]->scaleX;
      finished += done;
      ((GfxSprite **)self->base.data)[32 + i]->posY =
          ((GfxSprite **)self->base.data)[28 + i]->posY +
          self->namePanelOffset[i * 4 + 1] * ((GfxSprite **)self->base.data)[28 + i]->scaleY;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[32 + i],
                               ((GfxSprite **)self->base.data)[28 + i]->scaleX,
                               ((GfxSprite **)self->base.data)[28 + i]->scaleY, 0.0f);

      ((GfxSprite **)self->base.data)[30 + i]->alpha = ((GfxSprite **)self->base.data)[28 + i]->alpha;
      ((GfxSprite **)self->base.data)[30 + i]->posX =
          ((GfxSprite **)self->base.data)[28 + i]->posX +
          self->namePanelOffset[i * 4 + 2] * ((GfxSprite **)self->base.data)[28 + i]->scaleX;
      ((GfxSprite **)self->base.data)[30 + i]->posY =
          ((GfxSprite **)self->base.data)[28 + i]->posY +
          self->namePanelOffset[i * 4 + 3] * ((GfxSprite **)self->base.data)[28 + i]->scaleY;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[30 + i],
                               ((GfxSprite **)self->base.data)[28 + i]->scaleX,
                               ((GfxSprite **)self->base.data)[28 + i]->scaleY, 0.0f);
    }
  } else {
    for (i = 0; i < 2; i++) {
      bool done = UiTweenUpdate(1.2f, 1.0f, 16.0f, self->namePanelsHide,
                                ((GfxSprite **)self->base.data)[28 + i], &self->tweens[28 + i], 1);

      ((GfxSprite **)self->base.data)[32 + i]->alpha = ((GfxSprite **)self->base.data)[28 + i]->alpha;
      finished += done;
      ((GfxSprite **)self->base.data)[30 + i]->alpha = ((GfxSprite **)self->base.data)[28 + i]->alpha;
    }
  }
  if (finished != 0) {
    self->namePanelsDir = 0;
  }
}
