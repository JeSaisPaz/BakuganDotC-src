// bdc 0x0882cd4c UiCaptionSetText
#include "bdc.h"

/* Shows or hides the caption line of the 0x6e battle HUD / talk task (`BtlHud`): with
   `show == 0` it ends the current caption (`hintState = 100` if one is active per
   `UiCaptionIsShowing`, else clears window flag 0xe); with `show != 0` and `text` shorter than
   64 bytes it sets window flag 0xe, `hintTimer = duration`, centres the line (`captionX = 240 -
   strlen * 3.5`), encodes `text` into `captionText` (`UiTextEncodeSjis`), starts the caption
   (`hintState = 1`) and stores `hintPage = arg`; a longer `text` only clears flag 0xe. */

void UiCaptionSetText(void *win, char show, s32 arg, const char *text, s32 duration)
{
    BtlHud *hud = (BtlHud *)win;
    s32 len;

    if (show == 0) {
        if (UiCaptionIsShowing((UiTalkTask *)win) == 0) {
            UiSetWindowActive(0xe, 0);
        } else {
            hud->hintState = 100;
        }
        return;
    }
    len = (s32)strlen(text);
    if (len >= 0x40) {
        UiSetWindowActive(0xe, 0);
        return;
    }
    UiSetWindowActive(0xe, 1);
    hud->hintTimer = duration;
    hud->captionX = 240.0f - (float)len * 3.5f;
    UiTextEncodeSjis((u8 *)hud->captionText, text);
    hud->hintState = 1;
    hud->hintPage = arg;
}
