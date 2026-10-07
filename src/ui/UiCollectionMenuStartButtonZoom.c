// bdc 0x08977508 UiCollectionMenuStartButtonZoom
#include "bdc.h"

/* Starts the zoom-in (`out` = 0) or zoom-out of the entry buttons of the current page of the
   collection top menu (task 311, `maybe_UiScreen311Ctor`; entries `"Card_light"`,
   `"Figure_light"`, `"Sphere_light"`, `"Theater_light"`, `"Mark_light"`, `"Reset_light"`, a
   rotating `"menu_itembox.gmo"` model; page `+0x503` 0 = main entries, 1 = sub-page; selection
   `screen+0x500[page]`, enabled-entry mask `+0x50c`) (`UiTweenBegin`), with the same colouring
   and labels as `UiCollectionMenuStartButtonSlide`. */

/* The screen's sprite table (`base.data`), re-read on every access like the original. */
#define SPRITE(i) (((GfxSprite **)self->base.data)[i])

static void ResetAddColor(GfxSprite *sprite)
{
  sprite->addColor[0] = 0.0f;
  sprite->addColor[1] = 0.0f;
  sprite->addColor[2] = 0.0f;
  sprite->addColor[3] = 1.0f;
}

static void DimSprite(GfxSprite *sprite)
{
  sprite->tint[0] = 0.5f;
  sprite->tint[1] = 0.5f;
  sprite->tint[2] = 0.5f;
  sprite->alpha = 0.0f;
}

void UiCollectionMenuStartButtonZoom(UiCollectionMenu *self, u8 out)
{
  int i;
  int count;

  if (out == 0) {
    if (self->page == 0) {
      /* main entries 0..2 (sprites 3..5) */
      for (i = 3; i < 6; i++) {
        SPRITE(i)->flags |= 1;
        if ((self->entryMask & (1 << (i - 3))) == 0) {
          DimSprite(SPRITE(i));
        }
        ResetAddColor(SPRITE(i));
        SPRITE(i)->posX = self->entryX[i - 3];
        SPRITE(i)->posZ = self->spriteZ[i];
        UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
      }
      /* button frames 13..17 */
      for (i = 13; i < 18; i++) {
        UiCollectionMenuSetButtonFrame(self, SPRITE(i), 0);
        SPRITE(i)->flags |= 1;
        ResetAddColor(SPRITE(i));
        SPRITE(i)->posX = self->entryX[i - 13];
        SPRITE(i)->posZ = self->spriteZ[i];
        if (self->hideFifth != 0 && i == 17) {
          SPRITE(i)->flags &= ~1u;
        }
        UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
      }
      /* main entries 3..4 (sprites 10..11) */
      for (i = 10; i < 12; i++) {
        SPRITE(i)->flags |= 1;
        if (self->hideFifth != 0 && i == 11) {
          SPRITE(i)->flags &= ~1u;
        }
        if ((self->entryMask & (1 << (i - 7))) == 0) {
          DimSprite(SPRITE(i));
        }
        ResetAddColor(SPRITE(i));
        SPRITE(i)->posX = self->entryX[i - 10];
        SPRITE(i)->posZ = self->spriteZ[i];
        UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
      }
      /* hide the sub-page sprites */
      for (i = 1; i < 2; i++) {
        SPRITE(i)->flags &= ~1u;
      }
      for (i = 6; i < 10; i++) {
        SPRITE(i)->flags &= ~1u;
      }
    } else {
      /* sub-page title (sprite 1) */
      for (i = 1; i < 2; i++) {
        UiCollectionMenuSetEntryLabel(self, SPRITE(i), (u8)self->selMain);
        SPRITE(i)->flags |= 1;
        ResetAddColor(SPRITE(i));
        SPRITE(i)->posX = self->entryX[i - 1];
        SPRITE(i)->posZ = self->spriteZ[i];
        UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
      }
      count = UiCollectionMenuGetSubEntryCount(self, (u8)self->selMain);
      /* sub entries (sprites 6..9) */
      for (i = 6; i < 10; i++) {
        if (i - 6 < count) {
          SPRITE(i)->flags |= 1;
          UiCollectionMenuSetSubEntryLabel(self, SPRITE(i), (u8)self->selMain, (u8)(i - 6));
        } else {
          SPRITE(i)->flags &= ~1u;
        }
        ResetAddColor(SPRITE(i));
        SPRITE(i)->posX = self->entryX[i - 6];
        SPRITE(i)->posZ = self->spriteZ[i];
        if ((self->subMasks[self->selMain] & (1 << (i - 6))) == 0) {
          DimSprite(SPRITE(i));
        }
        UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
      }
      /* button frames 13..16 */
      for (i = 13; i < 17; i++) {
        UiCollectionMenuSetButtonFrame(self, SPRITE(i), 0);
        if (i - 13 < count) {
          SPRITE(i)->flags |= 1;
        } else {
          SPRITE(i)->flags &= ~1u;
        }
        ResetAddColor(SPRITE(i));
        SPRITE(i)->posX = self->entryX[i - 13];
        SPRITE(i)->posZ = self->spriteZ[i];
        UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
      }
      /* hide the main-page sprites */
      for (i = 3; i < 6; i++) {
        SPRITE(i)->flags &= ~1u;
      }
      for (i = 10; i < 12; i++) {
        SPRITE(i)->flags &= ~1u;
      }
      for (i = 17; i < 18; i++) {
        SPRITE(i)->flags &= ~1u;
      }
    }
  } else if (self->page == 0) {
    for (i = 3; i < 6; i++) {
      UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
    }
    for (i = 13; i < 18; i++) {
      UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
    }
    for (i = 10; i < 12; i++) {
      UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
    }
  } else {
    for (i = 1; i < 2; i++) {
      UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
    }
    for (i = 6; i < 10; i++) {
      UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
    }
    for (i = 13; i < 17; i++) {
      UiTweenBegin(1.0f, out, SPRITE(i), &self->tweens[i], 3);
    }
  }
}

#undef SPRITE
