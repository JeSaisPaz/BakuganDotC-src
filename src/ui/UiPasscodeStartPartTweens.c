// bdc 0x0893dcf4 UiPasscodeStartPartTweens
#include "bdc.h"

/* Starts the open (`out` = 0) or close (`out` = 1) tween of every part of the sequence-code screen
   (task 374, `UiPasscodeCtor`; the player re-enters a sequence of up to 6 symbols from a
   10-symbol pad and it is compared with the answer): makes visible (flag 1) the 10 symbol buttons
   (sprites 1..10, alpha reset to 1) and their icons (sprites 0x14..0x1d, sheet cell `i/5, i%5`),
   the two command buttons (0xb/0xc) and their labels (0x20/0x21), the entry frame (0xd), the
   result stamps (0x1e/0x1f) and sprite 0x26; the entry slots 0xe..0x13 are tweened without being
   shown. Each part is set up by `UiPasscodeBeginPartTween(screen, out, partIndex)`. */

void UiPasscodeStartPartTweens(UiScreen *screen, bool out)
{
  int i;

  for (i = 1; i < 0xb; i++) {
    ((GfxSprite **)screen->data)[i]->flags |= 1;
    ((GfxSprite **)screen->data)[i]->alpha = 1.0f;
    UiPasscodeBeginPartTween(screen, out, (u8)i);
  }
  for (i = 0x14; i < 0x1e; i++) {
    ((GfxSprite **)screen->data)[i]->flags |= 1;
    GfxSpriteSetCell(((GfxSprite **)screen->data)[i], (float)((i - 0x14) / 5),
                     (float)((i - 0x14) % 5));
    UiPasscodeBeginPartTween(screen, out, (u8)i);
  }
  for (i = 0xb; i < 0xd; i++) {
    ((GfxSprite **)screen->data)[i]->flags |= 1;
    UiPasscodeBeginPartTween(screen, out, (u8)i);
  }
  for (i = 0x20; i < 0x22; i++) {
    ((GfxSprite **)screen->data)[i]->flags |= 1;
    UiPasscodeBeginPartTween(screen, out, (u8)i);
  }
  ((GfxSprite **)screen->data)[0xd]->flags |= 1;
  UiPasscodeBeginPartTween(screen, out, 0xd);
  for (i = 0x1e; i < 0x20; i++) {
    ((GfxSprite **)screen->data)[i]->flags |= 1;
    UiPasscodeBeginPartTween(screen, out, (u8)i);
  }
  for (i = 0xe; i < 0x14; i++) {
    UiPasscodeBeginPartTween(screen, out, (u8)i);
  }
  for (i = 0x26; i < 0x27; i++) {
    ((GfxSprite **)screen->data)[i]->flags |= 1;
    UiPasscodeBeginPartTween(screen, out, (u8)i);
  }
}
