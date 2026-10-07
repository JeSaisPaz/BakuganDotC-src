// bdc 0x0893e46c UiPasscodeUpdateCursorPulse
#include "bdc.h"

/* Grows the highlighted button of the sequence-code screen (task 374, `UiPasscodeCtor`; the
   player re-enters a sequence of up to 6 symbols from a 10-symbol pad and it is compared with the
   answer) towards 1.1x: `cursorPulse` ramps to 1 in 0.2 steps and the scale is 1 + 0.1 * pulse,
   capped at 1.1. On the symbol pad it scales the focused symbol button (sprite 1 + focusSymbol),
   its icon (sprite 0x14 + focusSymbol) and the pad cursor (sprite 0); on the command row the
   selected command button (sprite 0xb + command), its label (sprite 0x20 + command) and the
   command cursor (sprite 0x25, also made visible); each is pulled to the front (posZ -100 / -102
   / -101). */

void UiPasscodeUpdateCursorPulse(UiScreen *screen)
{
  UiPasscode *pc = (UiPasscode *)screen;
  GfxSprite **sprites = (GfxSprite **)screen->data;
  float pulse = pc->cursorPulse;
  float scale;

  if (pulse < 1.0f) {
    pulse = pulse + 0.2f;
    pc->cursorPulse = pulse;
  }
  scale = pulse * 0.100000024f + 1.0f;
  if (!(scale <= 1.1f)) {
    scale = 1.1f;
  }
  if (pc->onCommandRow == 0) {
    UiSpriteSetScaleRotation(sprites[1 + pc->focusSymbol], scale, scale, 0.0f);
    ((GfxSprite **)screen->data)[1 + pc->focusSymbol]->posZ = -100.0f;
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[0x14 + pc->focusSymbol], scale, scale, 0.0f);
    ((GfxSprite **)screen->data)[0x14 + pc->focusSymbol]->posZ = -102.0f;
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[0], scale, scale, 0.0f);
    ((GfxSprite **)screen->data)[0]->posZ = -101.0f;
  } else {
    UiSpriteSetScaleRotation(sprites[0xb + pc->command], scale, scale, 0.0f);
    ((GfxSprite **)screen->data)[0xb + pc->command]->posZ = -100.0f;
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[0x20 + pc->command], scale, scale, 0.0f);
    ((GfxSprite **)screen->data)[0x20 + pc->command]->posZ = -102.0f;
    ((GfxSprite **)screen->data)[0x25]->flags |= 1;
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[0x25], scale, scale, 0.0f);
    ((GfxSprite **)screen->data)[0x25]->posZ = -101.0f;
  }
}
