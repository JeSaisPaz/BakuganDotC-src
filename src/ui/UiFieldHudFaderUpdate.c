// bdc 0x088c8e98 UiFieldHudFaderUpdate
#include "bdc.h"

/* Per-frame step of the field HUD fader (`UiFieldHudFader` behind `UiFieldHud.fader`): state 1
   lowers alpha by 0.1 until it reaches 0, state 2 raises it by 0.1 until it reaches 1 (either then
   returns to idle state 0; other states leave alpha alone), and writes the alpha into 16 sprites of
   the bound HUD sprite table. Called by `UiFieldHudStage32Phase`. */

void UiFieldHudFaderUpdate(void **fader)

{
  UiFieldHudFader *f;
  float alpha;

  f = (UiFieldHudFader *)*fader;
  alpha = f->alpha;
  if (f->state > 0) {
    if (f->state < 2) {
      alpha = alpha - 0.1f;
      f->alpha = alpha;
      if (alpha <= 0.0f) {
        f->alpha = 0.0f;
        alpha = 0.0f;
        f->state = 0;
      }
    }
    else if (f->state < 3) {
      alpha = alpha + 0.1f;
      f->alpha = alpha;
      if (!(alpha < 1.0f)) {
        f->alpha = 1.0f;
        alpha = 1.0f;
        f->state = 0;
      }
    }
  }
  (*f->sprites)[30]->alpha = alpha;
  (*f->sprites)[40]->alpha = alpha;
  (*f->sprites)[68]->alpha = alpha;
  (*f->sprites)[2]->alpha = alpha;
  (*f->sprites)[20]->alpha = alpha;
  (*f->sprites)[28]->alpha = alpha;
  (*f->sprites)[4]->alpha = alpha;
  (*f->sprites)[18]->alpha = alpha;
  (*f->sprites)[21]->alpha = alpha;
  (*f->sprites)[3]->alpha = alpha;
  (*f->sprites)[19]->alpha = alpha;
  (*f->sprites)[27]->alpha = alpha;
  (*f->sprites)[17]->alpha = alpha;
  (*f->sprites)[5]->alpha = alpha;
  (*f->sprites)[29]->alpha = alpha;
  (*f->sprites)[39]->alpha = alpha;
  return;
}
