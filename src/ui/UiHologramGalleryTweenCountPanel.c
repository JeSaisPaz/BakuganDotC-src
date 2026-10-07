// bdc 0x0891ff88 UiHologramGalleryTweenCountPanel
#include "bdc.h"

/* Starts the open/close slide tweens of the counter panel of the hologram gallery screen (task 391,
   `maybe_UiScreen391Ctor`, class prefix `UiHologramGallery`; `fix_*` sprites, `DWHologramHelp`
   texts; main update `UiHologramGalleryMainPhase` with step `+0x2c`; placed holograms are save-profile bytes
   `+0x84 + slot`). The panel is sprites 30..50 in three groups of seven (30..36, 37..43, 44..50).
   Opening (`hide == 0`): each group first gets its number with `UiHologramGallerySetNumber`
   (profile `points` at sprite 36; profile word 0x2d at sprite 43 in colour 1; `points` minus word
   0x2d at sprite 50), then its sprites get alpha 0 and slide in Y from -32 to 0. Closing: all
   sprites slide in Y from 0 to -32 and fade out. */

#define SPRITES(self) ((GfxSprite **)(self)->base.data)

void UiHologramGalleryTweenCountPanel(UiHologramGallery *self, u8 hide)
{
  s32 i;
  s32 points;
  u32 word;

  if (hide == 0) {
    points = SaveGetProfile()->data->points;
    UiHologramGallerySetNumber(self, points, 0x24, 0, SPRITES(self)[0x24]->posX,
                               SPRITES(self)[0x24]->posY);
    for (i = 0x1e; i < 0x25; i++) {
      SPRITES(self)[i]->alpha = 0.0f;
      UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, SPRITES(self)[i], &self->tweens[i], 9);
    }
    word = SaveProfileGetWord(SaveGetProfile(), 0x2d);
    UiHologramGallerySetNumber(self, (s32)word, 0x2b, 1, SPRITES(self)[0x2b]->posX,
                               SPRITES(self)[0x2b]->posY);
    for (i = 0x25; i < 0x2c; i++) {
      SPRITES(self)[i]->alpha = 0.0f;
      UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, SPRITES(self)[i], &self->tweens[i], 9);
    }
    points = SaveGetProfile()->data->points;
    word = SaveProfileGetWord(SaveGetProfile(), 0x2d);
    UiHologramGallerySetNumber(self, points - (s32)word, 0x32, 0, SPRITES(self)[0x32]->posX,
                               SPRITES(self)[0x32]->posY);
    for (i = 0x2c; i < 0x33; i++) {
      SPRITES(self)[i]->alpha = 0.0f;
      UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, SPRITES(self)[i], &self->tweens[i], 9);
    }
  } else {
    for (i = 0x1e; i < 0x25; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, SPRITES(self)[i], &self->tweens[i], 9);
    }
    for (i = 0x25; i < 0x2c; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, SPRITES(self)[i], &self->tweens[i], 9);
    }
    for (i = 0x2c; i < 0x33; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, SPRITES(self)[i], &self->tweens[i], 9);
    }
  }
}
