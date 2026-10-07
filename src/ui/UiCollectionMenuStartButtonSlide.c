// bdc 0x089766fc UiCollectionMenuStartButtonSlide
#include "bdc.h"

/* Starts the slide-in (`out` = 0) or slide-out of the entry buttons of the current page of the
   collection top menu (task 311, `maybe_UiScreen311Ctor`; entries `"Card_light"`,
   `"Figure_light"`, `"Sphere_light"`, `"Theater_light"`, `"Mark_light"`, `"Reset_light"`, a
   rotating `"menu_itembox.gmo"` model; page `page` 0 = main entries, 1 = sub-page; selection
   `selMain`, enabled-entry mask `entryMask`) (`UiTweenBeginSlide`): shows them, dims the
   disabled ones (mask `entryMask` / `subMasks[selMain]`) to 50 % grey, sets their labels
   (`UiCollectionMenuSetButtonFrame` `"waku_1_a"`/`"waku_1_b"`, `UiCollectionMenuGetSubEntryCount`,
   `UiCollectionMenuSetEntryLabel`, `UiCollectionMenuSetSubEntryLabel`). Slide-in moves every
   button from -128 to 0 on X; slide-out moves it from 0 to -128 and fades it. */

/* The sprite table is re-read from `base.data` on every access, as the asm does. */
static GfxSprite *Sprite(UiCollectionMenu *self, int i)
{
  return ((GfxSprite **)self->base.data)[i];
}

static void SetTint(GfxSprite *sprite, float v)
{
  sprite->tint[0] = v;
  sprite->tint[1] = v;
  sprite->tint[2] = v;
  sprite->alpha = 0.0f;
}

static void ClearAddColor(GfxSprite *sprite)
{
  sprite->addColor[0] = 0.0f;
  sprite->addColor[1] = 0.0f;
  sprite->addColor[2] = 0.0f;
  sprite->addColor[3] = 1.0f;
}

void UiCollectionMenuStartButtonSlide(UiCollectionMenu *self, u8 out)

{
  int i;
  int count;
  u8 entry;

  if (out == 0) {
    if (self->page == 0) {
      /* main page: entry lights 3..5 */
      for (i = 3; i < 6; i++) {
        Sprite(self, i)->flags |= 1;
        if ((self->entryMask & (1 << (i - 3))) == 0) {
          SetTint(Sprite(self, i), 0.5f);
        } else {
          SetTint(Sprite(self, i), 1.0f);
        }
        ClearAddColor(Sprite(self, i));
        Sprite(self, i)->posX = self->entryX[i - 3];
        Sprite(self, i)->posZ = self->spriteZ[i];
        UiTweenBeginSlide(1.0f, -128.0f, 0.0f, out, Sprite(self, i), &self->tweens[i], 7);
      }
      /* button frames 13..17 */
      for (i = 13; i < 18; i++) {
        UiCollectionMenuSetButtonFrame(self, Sprite(self, i), 0);
        Sprite(self, i)->flags |= 1;
        ClearAddColor(Sprite(self, i));
        Sprite(self, i)->posX = self->entryX[i - 13];
        Sprite(self, i)->posZ = self->spriteZ[i];
        if (self->hideFifth != 0 && i == 17) {
          Sprite(self, i)->flags &= ~1u;
        }
        UiTweenBeginSlide(1.0f, -128.0f, 0.0f, out, Sprite(self, i), &self->tweens[i], 7);
      }
      /* entry lights 10..11 (mask bits 3..4) */
      for (i = 10; i < 12; i++) {
        Sprite(self, i)->flags |= 1;
        if (self->hideFifth != 0 && i == 11) {
          Sprite(self, i)->flags &= ~1u;
        }
        if ((self->entryMask & (1 << (i - 7))) == 0) {
          SetTint(Sprite(self, i), 0.5f);
        } else {
          SetTint(Sprite(self, i), 1.0f);
        }
        ClearAddColor(Sprite(self, i));
        Sprite(self, i)->posX = self->entryX[i - 10];
        Sprite(self, i)->posZ = self->spriteZ[i];
        UiTweenBeginSlide(1.0f, -128.0f, 0.0f, out, Sprite(self, i), &self->tweens[i], 7);
      }
      /* hide the sub-page sprites */
      for (i = 1; i < 2; i++) {
        Sprite(self, i)->flags &= ~1u;
      }
      for (i = 6; i < 10; i++) {
        Sprite(self, i)->flags &= ~1u;
      }
    } else {
      /* sub-page: header label 1 */
      entry = (u8)self->selMain;
      for (i = 1; i < 2; i++) {
        UiCollectionMenuSetEntryLabel(self, Sprite(self, i), entry);
        Sprite(self, i)->flags |= 1;
        ClearAddColor(Sprite(self, i));
        Sprite(self, i)->posX = self->entryX[i - 1];
        Sprite(self, i)->posZ = self->spriteZ[i];
        UiTweenBeginSlide(1.0f, -128.0f, 0.0f, out, Sprite(self, i), &self->tweens[i], 7);
        entry = (u8)self->selMain;
      }
      count = UiCollectionMenuGetSubEntryCount(self, entry);
      /* sub-entry labels 6..9 */
      for (i = 6; i < 10; i++) {
        if (i - 6 < count) {
          Sprite(self, i)->flags |= 1;
          UiCollectionMenuSetSubEntryLabel(self, Sprite(self, i), (u8)self->selMain, (u8)(i - 6));
        } else {
          Sprite(self, i)->flags &= ~1u;
        }
        ClearAddColor(Sprite(self, i));
        Sprite(self, i)->posX = self->entryX[i - 6];
        Sprite(self, i)->posZ = self->spriteZ[i];
        if ((self->subMasks[self->selMain] & (1 << (i - 6))) == 0) {
          SetTint(Sprite(self, i), 0.5f);
        } else {
          SetTint(Sprite(self, i), 1.0f);
        }
        UiTweenBeginSlide(1.0f, -128.0f, 0.0f, out, Sprite(self, i), &self->tweens[i], 7);
      }
      /* button frames 13..16 */
      for (i = 13; i < 17; i++) {
        UiCollectionMenuSetButtonFrame(self, Sprite(self, i), 0);
        if (i - 13 < count) {
          Sprite(self, i)->flags |= 1;
        } else {
          Sprite(self, i)->flags &= ~1u;
        }
        ClearAddColor(Sprite(self, i));
        Sprite(self, i)->posX = self->entryX[i - 13];
        Sprite(self, i)->posZ = self->spriteZ[i];
        UiTweenBeginSlide(1.0f, -128.0f, 0.0f, out, Sprite(self, i), &self->tweens[i], 7);
      }
      /* hide the main-page sprites */
      for (i = 3; i < 6; i++) {
        Sprite(self, i)->flags &= ~1u;
      }
      for (i = 10; i < 12; i++) {
        Sprite(self, i)->flags &= ~1u;
      }
      for (i = 17; i < 18; i++) {
        Sprite(self, i)->flags &= ~1u;
      }
    }
  } else if (self->page == 0) {
    for (i = 3; i < 6; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, -128.0f, out, Sprite(self, i), &self->tweens[i], 7);
    }
    for (i = 13; i < 18; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, -128.0f, out, Sprite(self, i), &self->tweens[i], 7);
    }
    for (i = 10; i < 12; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, -128.0f, out, Sprite(self, i), &self->tweens[i], 7);
    }
  } else {
    for (i = 1; i < 2; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, -128.0f, out, Sprite(self, i), &self->tweens[i], 7);
    }
    for (i = 6; i < 10; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, -128.0f, out, Sprite(self, i), &self->tweens[i], 7);
    }
    for (i = 13; i < 17; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, -128.0f, out, Sprite(self, i), &self->tweens[i], 7);
    }
  }
  return;
}
