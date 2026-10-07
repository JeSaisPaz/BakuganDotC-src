// bdc 0x08835af0 UiTalkWindowUpdate
#include "bdc.h"

/* HUD widget updater (`BtlHudPhaseMain`) for the battle talk window: when the text printer
   `+0x688` and the player Bakugan (`BtlHudGetPlayerBakugan`) exist, accumulates changes of the
   unit word `+0x580` into `+0x904` (last value `+0x908`), steps the window (`UiTalkWindowStep`)
   and, when the text alpha `+0x8b4` or `+0x8bc` changed, copies the alpha to both printer layers
   (`+0x688` and `+0x68c`, field `+0x70`). */

void UiTalkWindowUpdate(void *win)

{
  BtlHud *hud = (BtlHud *)win;
  BtlBakugan *player;
  int count;

  if (hud->overlayObj[0] != NULL && (player = (BtlBakugan *)BtlHudGetPlayerBakugan(hud)) != NULL) {
    count = player->knockoutCount;
    if (hud->talkUnitLast != count) {
      hud->talkUnitDelta = hud->talkUnitDelta + (count - hud->talkUnitLast);
      hud->talkUnitLast = count;
    }
    UiTalkWindowStep(win);
    if (hud->talkTextAlphaSet == hud->talkTextAlpha) {
      if (hud->talkOverlayAlphaSet == hud->talkOverlayAlpha) {
        return;
      }
    }
    ((GfxSpriteLayer *)hud->overlayObj[0])->alpha = hud->talkTextAlpha;
    ((GfxSpriteLayer *)hud->overlayObj[1])->alpha = hud->talkTextAlpha;
    hud->talkTextAlphaSet = hud->talkTextAlpha;
    hud->talkOverlayAlphaSet = hud->talkOverlayAlpha;
  }
}
