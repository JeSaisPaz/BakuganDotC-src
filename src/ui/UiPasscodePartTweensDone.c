// bdc 0x0893e24c UiPasscodePartTweensDone
#include "bdc.h"

/* Advances all part tweens of `UiPasscode` with `UiPasscodeUpdatePartTween`
   in this order: parts 1..10, the symbol pad 0x14..0x1d (whose cells are first reset to symbol
   i on a 5-column sheet), 0x0b..0x0c, 0x20..0x21, 0x0d, 0x1e..0x1f, 0x0e..0x13 and 0x26.
   The finished flags are summed in a u8; returns true when that sum is non-zero, i.e. once
   at least one part reports finished (all parts tween in lockstep). */

bool UiPasscodePartTweensDone(UiScreen *screen, bool closing)
{
    u8 finished = 0;
    int part;

    for (part = 1; part < 0xb; part++)
        finished += UiPasscodeUpdatePartTween(screen, closing, (u8)part);
    for (part = 0x14; part < 0x1e; part++) {
        GfxSpriteSetCell(((GfxSprite **)screen->data)[part], (float)((part - 0x14) / 5),
                         (float)((part - 0x14) % 5));
        finished += UiPasscodeUpdatePartTween(screen, closing, (u8)part);
    }
    for (part = 0xb; part < 0xd; part++)
        finished += UiPasscodeUpdatePartTween(screen, closing, (u8)part);
    for (part = 0x20; part < 0x22; part++)
        finished += UiPasscodeUpdatePartTween(screen, closing, (u8)part);
    finished += UiPasscodeUpdatePartTween(screen, closing, 0xd);
    for (part = 0x1e; part < 0x20; part++)
        finished += UiPasscodeUpdatePartTween(screen, closing, (u8)part);
    for (part = 0xe; part < 0x14; part++)
        finished += UiPasscodeUpdatePartTween(screen, closing, (u8)part);
    for (part = 0x26; part < 0x27; part++)
        finished += UiPasscodeUpdatePartTween(screen, closing, (u8)part);
    return finished != 0;
}
