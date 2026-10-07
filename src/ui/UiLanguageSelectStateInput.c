// bdc 0x0880a130 UiLanguageSelectStateInput
#include "bdc.h"

/* Input state of the language-selection screen. Sub-step 0: up/down (pad repeat bits 0x10/0x40)
   moves `cursor` over the 7 languages with wrap-around and a cursor sound (`SndManagerPlay` 1);
   cross (0x4000) stores the language with `SaveProfileSetLanguage` (from
   `g_uiLanguageSelectLangIds`), plays sound 0, reports 1 through `UiLanguageSelectSetResult` and
   goes to sub-step 1; circle (0x2000) plays a buzzer (sound 3) when `word5c` is 0, or when it is 1
   plays sound 2, reports -1 and jumps to sub-step 2. Sub-step 1 speeds the tint pulse up for 21
   frames, then moves to sub-step 2. Any other sub-step switches the screen to state 3, sub-step 1.
   Every frame ends with `UiLanguageSelectAnimateItems` and a `frameCount` increment. */

void UiLanguageSelectStateInput(CoreTask *task)

{
  UiLanguageSelect *self = (UiLanguageSelect *)task;
  PadState *pad;
  s32 mode;
  s32 delta;
  s32 cursor;

  if (self->subStep > 0) {
    if (self->subStep < 2) {
      self->frameCount += 0xc;
      self->closeTimer++;
      if (self->closeTimer >= 0x15) {
        self->subStep = 2;
      }
      goto animate;
    }
  } else if (self->subStep >= 0) {
    pad = self->pad;
    if ((pad->repeat & 0x2000) != 0) {
      mode = (s32)(intptr_t)self->word5c;
      if (mode > 0) {
        if (mode < 2) {
          self->subStep = 2;
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 2, 0, 0);
          }
          UiLanguageSelectSetResult(self, -1);
        }
      } else if (mode >= 0) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 3, 0, 0);
        }
      }
    } else if ((pad->repeat & 0x4000) != 0) {
      self->subStep = 1;
      SaveProfileSetLanguage(SaveGetProfile(), g_uiLanguageSelectLangIds[self->cursor]);
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiLanguageSelectSetResult(self, 1);
      self->frameCount = 0;
      self->closeTimer = 0;
    } else {
      delta = 0;
      if ((pad->repeat & 0x10) != 0) {
        delta = -1;
      } else if ((pad->repeat & 0x40) != 0) {
        delta = 1;
      }
      if (delta != 0) {
        cursor = delta + self->cursor;
        if (cursor < 0) {
          cursor = 6;
        }
        if (cursor >= 7) {
          cursor = 0;
        }
        self->cursor = cursor;
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        self->frameCount = 0;
      }
    }
    goto animate;
  }
  self->state = 3;
  self->subStep = 1;
animate:
  UiLanguageSelectAnimateItems(self);
  self->frameCount++;
}
