// bdc 0x08936ee0 UiGauntletSetupUpdateCardInfo
#include "bdc.h"

/* Runs the card information panel of `UiGauntletSetup` (task 373) while
   `cardInfo[0]` is non-zero; `cardInfo[1]` is the panel step. In mode 1, when the focused slot
   (`item`) changes, a filled slot restarts the panel at step 1 whatever the focus area, an empty
   slot (`slotCard == 0xff`) hides it; when `item` is unchanged but the focus area changes, a
   filled slot in focus area 0 restarts it and anything else hides it. Hiding clears panel
   sprites 19 and 46 at once, zeroes the name/help alphas and resets the step to 0. In any other mode, a step
   below 3 jumps to 3 (fade out) when the focused slot holds a card, else to 5.
   Steps: 1 shows sprites 19/46, gives 46 the card's large image
   (`UiGauntletSetupSetCardTexture`), places them 64 px right of their rest X and starts the
   slide-in (`UiTweenBeginSlide`), offsets sprite 54 the same way, prints the card name and help
   (`UiGauntletSetupSetCardName`, `UiGauntletSetupSetCardHelp`); 2 advances the slide-in
   (`UiTweenUpdate`) and slides the name/help glyphs along with it, back to step 0 once a tween
   reports done; 3 starts the fade-out (`UiTweenBegin`); 4 advances it, going to step 5 when done;
   step 5 or above clears `cardInfo[0]`. Always ends by latching `item`/`focusArea` into
   `prevItem`/`prevFocusArea` (except when inactive on entry). */

#define CARDINFO_SPRITE(self, i) (((GfxSprite **)(self)->base.data)[i])

void UiGauntletSetupUpdateCardInfo(UiGauntletSetup *self)
{
  u8 done = 0;
  bool hide;
  u8 step;
  int i;
  int j;
  GfxSprite *glyph;
  GfxSprite *sprite;
  float u;

  if (self->cardInfo[0] == 0) {
    return;
  }
  if (self->cardInfo[0] == 1) {
    hide = false;
    if (self->prevItem != self->item) {
      if (self->slotCard[self->item] == 0xff) {
        hide = true;
      } else {
        self->cardInfo[1] = 1;
      }
    } else if (self->focusArea != self->prevFocusArea) {
      if (self->focusArea == 0) {
        if (self->slotCard[self->item] == 0xff) {
          hide = true;
        } else {
          self->cardInfo[1] = 1;
        }
      } else {
        hide = true;
      }
    }
    if (hide) {
      for (i = 19; i < 20; i++) {
        CARDINFO_SPRITE(self, i)->flags &= ~1u;
        CARDINFO_SPRITE(self, i)->alpha = 0.0f;
      }
      for (i = 46; i < 47; i++) {
        CARDINFO_SPRITE(self, i)->flags &= ~1u;
        CARDINFO_SPRITE(self, i)->alpha = 0.0f;
      }
      self->nameAlpha = 0.0f;
      self->helpAlpha = 0.0f;
      self->nameAppliedAlpha = 1.0f;
      self->helpAppliedAlpha = 1.0f;
      self->cardInfo[1] = 0;
    }
  } else if (self->cardInfo[1] < 3) {
    step = 5;
    if (self->slotCard[self->item] != 0xff) {
      step = 3;
    }
    self->cardInfo[1] = step;
  }

  step = self->cardInfo[1];
  if (step >= 5) {
    self->cardInfo[0] = 0;
  } else if (step == 1) {
    for (i = 19; i < 20; i++) {
      CARDINFO_SPRITE(self, i)->flags |= 1;
      CARDINFO_SPRITE(self, i)->alpha = 0.0f;
      CARDINFO_SPRITE(self, i)->layerMask = 2;
      CARDINFO_SPRITE(self, i)->posX = self->spritePos[i][0] + 64.0f;
      sprite = CARDINFO_SPRITE(self, i);
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[i][0] - sprite->posX, 0, sprite,
                        &self->tweens[i], 7);
    }
    for (i = 46; i < 47; i++) {
      CARDINFO_SPRITE(self, i)->flags |= 1;
      CARDINFO_SPRITE(self, i)->alpha = 0.0f;
      CARDINFO_SPRITE(self, i)->layerMask = 2;
      UiGauntletSetupSetCardTexture(CARDINFO_SPRITE(self, i), self->slotCard[self->item]);
      CARDINFO_SPRITE(self, i)->posX = self->spritePos[i][0] + 64.0f;
      sprite = CARDINFO_SPRITE(self, i);
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[i][0] - sprite->posX, 0, sprite,
                        &self->tweens[i], 7);
    }
    for (i = 54; i < 55; i++) {
      CARDINFO_SPRITE(self, i)->posX = self->spritePos[i][0] + 64.0f;
    }
    UiGauntletSetupSetCardName(self, self->slotCard[self->item]);
    UiGauntletSetupSetCardHelp(self, self->slotCard[self->item]);
    self->cardInfo[1]++;
  } else if (step == 2) {
    for (i = 19; i < 20; i++) {
      done += UiTweenUpdate(1.0f, 1.0f, 16.0f, 0, CARDINFO_SPRITE(self, i), &self->tweens[i], 5);
    }
    for (i = 46; i < 47; i++) {
      done += UiTweenUpdate(1.0f, 1.0f, 16.0f, 0, CARDINFO_SPRITE(self, i), &self->tweens[i], 5);
    }
    self->nameAlpha = CARDINFO_SPRITE(self, 46)->alpha;
    glyph = self->nameGlyphs;
    if (0.0f < self->nameReveal) {
      j = 0;
      do {
        u = self->tweens[46].t - 1.0f;
        glyph->posX = self->nameGlyphX[j] - (1.0f - u * u) * 64.0f;
        j++;
        glyph = glyph->next;
      } while ((float)j < self->nameReveal);
    }
    self->helpAlpha = CARDINFO_SPRITE(self, 46)->alpha;
    glyph = self->helpGlyphs;
    if (0.0f < self->helpReveal) {
      j = 0;
      do {
        u = self->tweens[46].t - 1.0f;
        glyph->posX = self->helpGlyphX[j] - (1.0f - u * u) * 64.0f;
        j++;
        glyph = glyph->next;
      } while ((float)j < self->helpReveal);
    }
    if (done != 0) {
      self->cardInfo[1] = 0;
    }
  } else if (step == 3) {
    for (i = 19; i < 20; i++) {
      UiTweenBegin(1.0f, 1, CARDINFO_SPRITE(self, i), &self->tweens[i], 1);
    }
    for (i = 46; i < 47; i++) {
      UiTweenBegin(1.0f, 1, CARDINFO_SPRITE(self, i), &self->tweens[i], 1);
    }
    self->cardInfo[1]++;
  } else if (step == 4) {
    for (i = 19; i < 20; i++) {
      done += UiTweenUpdate(1.0f, 1.0f, 16.0f, 1, CARDINFO_SPRITE(self, i), &self->tweens[i], 1);
    }
    for (i = 46; i < 47; i++) {
      done += UiTweenUpdate(1.0f, 1.0f, 16.0f, 1, CARDINFO_SPRITE(self, i), &self->tweens[i], 1);
    }
    self->nameAlpha = CARDINFO_SPRITE(self, 46)->alpha;
    self->helpAlpha = CARDINFO_SPRITE(self, 46)->alpha;
    if (done != 0) {
      self->cardInfo[1] = 5;
    }
  }
  self->prevItem = self->item;
  self->prevFocusArea = self->focusArea;
}
