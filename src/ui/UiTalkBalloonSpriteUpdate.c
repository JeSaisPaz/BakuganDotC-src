// bdc 0x088c8688 UiTalkBalloonSpriteUpdate
#include "bdc.h"

/* Update (vtable slot 2) of a talk balloon sprite (`UiTalkBalloonSprite`: `GfxSprite` subclass,
   vtable `0x08af2d34`, owner window `owner`, `kind`, `state`): dispatches on `kind`:
   0 `UiTalkBalloonGlyphUpdate`, 1 `UiTalkBalloonFrameUpdate`, 2
   `UiTalkBalloonCheckUpdate`, 3 `UiTalkBalloonCursorUpdate`, 4 `UiTalkBalloonFadeIconUpdate`;
   other kinds do nothing. */

void UiTalkBalloonSpriteUpdate(GfxSprite *sprite)
{
  u32 kind = (u32)((UiTalkBalloonSprite *)sprite)->kind;

  if (kind < 5) {
    switch (kind) {
    case 1: UiTalkBalloonFrameUpdate(sprite); return;
    case 2: UiTalkBalloonCheckUpdate(sprite); return;
    case 3: UiTalkBalloonCursorUpdate(sprite); return;
    case 4: UiTalkBalloonFadeIconUpdate(sprite); return;
    default: UiTalkBalloonGlyphUpdate(sprite); return;
    }
  }
}
