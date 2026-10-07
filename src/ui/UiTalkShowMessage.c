// bdc 0x0882c7a4 UiTalkShowMessage
#include "bdc.h"

/* Shows a message on the talk window of the HUD task `win` (`UiGetTalkTask`, id 0x6e): returns 0
   for `style == 4`, 1 for `msgId == -1`, and 0 when not `force` and a message is still active.
   Otherwise, when `faceId` changed, loads the face texture `"tips_kao%02d"` into sprite 0x81 at the
   position of layout 4 entry 0x81; resets the UV rect of sprites 0x82/0x83 and stores the frame for
   `faceId` (12 = the player Bakugan's attribute) in sprite 0x82. Unless `force`, the same
   (style, msgId) is not restarted. On (re)start lays out `msgId` (`UiTalkLayoutText`), sets the
   hold time, fades, voice line and ducking, and optionally decrements `talkUnitDelta`.
   Returns 1 when the message was (re)started, else 0. */

char UiTalkShowMessage(void *win, u32 faceId, s32 msgId, s32 force, s32 duration, s32 style, s32 arg7, s32 arg8, u32 decCounter, s32 voiceId)
{
    BtlHud *hud = (BtlHud *)win;
    u8 dec = (u8)decCounter;
    char restart;
    GfxSprite *face;
    BtlBakugan *player;
    s32 frame;
    s32 i;
    char name[32];

    if (style == 4) {
        return 0;
    }
    if (msgId == -1) {
        return 1;
    }
    face = hud->sprites[0x81];
    restart = 0;
    if (force == 0 && hud->talkState != 0) {
        return 0;
    }
    if ((u32)hud->talkKind != faceId) {
        s16 *entry = UiLayoutGetEntry(4, 0x81);

        sprintf(name, "tips_kao%02d", faceId);
        face->texture = GfxFindTexture(name);
        face->posX = (float)entry[0];
        face->posW = 0.0f;
        face->posY = (float)entry[1];
        face->posZ = (float)(entry[2] + 200);
        hud->talkKind = faceId;
    }
    for (i = 0x82; i < 0x84; i++) {
        BtlHudSetSpriteRect(0.0f, 0.0f, 128.0f, 96.0f, hud, hud->sprites[i]);
    }
    player = (BtlBakugan *)BtlHudGetPlayerBakugan(hud);
    frame = 0;
    switch (faceId) {
    case 0:  frame = 0; break;
    case 1:  frame = 5; break;
    case 2:  frame = 1; break;
    case 3:  frame = 4; break;
    case 4:  frame = 2; break;
    case 5:  frame = 3; break;
    case 6:  frame = 0; break;
    case 7:  frame = 1; break;
    case 8:  frame = 3; break;
    case 9:  frame = 2; break;
    case 10: frame = 4; break;
    case 11: frame = 5; break;
    case 12:
        if (player != NULL) {
            /* virtual slot 20 (`+0xa0`): the Bakugan's attribute */
            const VtblEntry *e = &((const VtblEntry *)player->base.base.vtable)[20];
            frame = ((s32 (*)(void *))e->fn)((u8 *)player + e->delta);
        }
        break;
    case 15: frame = 6; break;
    default: break;
    }
    hud->sprites[0x82]->textureSlot = frame;

    if (force != 0 || hud->talkStyle != style || hud->talkMsgId != msgId) {
        restart = 1;
    }
    if (restart != 0) {
        /* dead: style 4 already returned above */
        if (style == 4 && msgId < 5) {
            msgId = msgId + (s32)faceId * 5 + 0x1da;
        }
        hud->talkForce = force;
        hud->talkHold = duration;
        hud->talkPageHold = duration;
        UiTalkLayoutText(hud, msgId, (void *)hud->msgTable, arg7, arg8);
        hud->talkStyle = style;
        hud->talkMsgId = msgId;
        hud->talkTextAlpha = 0.0f;
        hud->talkTextAlphaSet = -1.0f;
        hud->talkOverlayAlpha = 1.0f;
        hud->talkOverlayAlphaSet = 1.0f;
        hud->talkState = 1;
        hud->talkScriptSuspended = 0;
        hud->talkVoiceId = voiceId;
        if (voiceId != -1) {
            if (voiceId < 10000) {
                voiceId = voiceId + 10000;
            }
            hud->talkVoiceId = voiceId;
            if (hud->talkForce == 1 && BtlStageReturnFalse() == 0) {
                hud->talkHold = 30;
                hud->talkPageHold = 30;
                hud->talkScriptSuspended = 1;
                BtlStageSuspendEventScript();
            }
            if (SndBgmPlayerExists(1) && SndBgmPlayVoice(hud->talkVoiceId) != 0 &&
                BtlCameraTaskExists() != 0) {
                BtlGetCameraTask();
                BtlSetDuckVolume(0.45f, 0.3f, 0.5f);
            }
        }
        if (dec != 0) {
            s32 n = hud->talkUnitDelta - 1;

            if (n < 0) {
                n = 0;
            } else if (999 < n) {
                n = 999;
            }
            hud->talkUnitDelta = n;
        }
    }
    return restart;
}
