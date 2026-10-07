// bdc 0x0881bd40 NetPlayLateUpdate
#include "bdc.h"

/* Second per-frame NetPlay step, called by `BootMainThread` right after the task update. Always
   sets `publishSkipped` first and returns unless `NetPlayUpdate` ran this frame (`updateRan`).
   In state 7 with a valid local character (`NetCharaGetByIndex`(0)) it clears `publishSkipped`
   and publishes the local status: with bit `0x8000000` clear in `NetPlayGetFlags` it runs
   `NetPlayExchangeEntries`; otherwise it fills a `NetCharaMsg` `input` record (flag word merged
   with `0x400000`/`0x200000` for the save-profile flags `0x2000`/`0x1000`, the buttons and stick of
   `remotePad` unless profile flag `0x3200` is set, and the yaw of `g_gfxActiveCamera`), sends it
   with `NetCharaPushMessage` and, if it was queued, re-polls the pad with `PadRead`. If not
   `synced` and `NetModeFlagIsClear` it marks the character ready (`NetCharaSetReady`), and it
   always commits the frame with `NetCharaCommitFrame`. Afterwards it requests an abort when task
   `0x15e` exists (`CoreTaskFind`). In state 7, while the disc manager exists and is busy, it
   counts frames in `umdWaitFrames` for which `IoUmdIsMediaReady` fails (reset when it passes):
   at 600 it sets save-profile flag `0x80` and requests an abort (`NetPlayRequestAbort`). */

void NetPlayLateUpdate(NetPlay *self)
{
    NetChara *chara;
    u32 flags;
    PadState *pad;
    float yaw;
    NetCharaMsg msg;

    self->publishSkipped = 1;
    if (!self->updateRan) {
        return;
    }
    if (self->state == 7 && (chara = NetCharaGetByIndex(0)) != NULL) {
        flags = NetPlayGetFlags(self);
        self->publishSkipped = 0;
        if ((flags & 0x8000000) != 0) {
            pad = self->remotePad;
            yaw = 0.0f;
            if (g_gfxActiveCamera != NULL) {
                yaw = g_gfxActiveCamera->yaw;
            }
            memset(&msg, 0, sizeof(msg));
            msg.flags = flags;
            if (SaveProfileHasFlags(SaveGetProfile(), 0x2000)) {
                msg.flags |= 0x400000;
            }
            if (SaveProfileHasFlags(SaveGetProfile(), 0x1000)) {
                msg.flags |= 0x200000;
            }
            if (SaveProfileHasFlags(SaveGetProfile(), 0x3200)) {
                msg.body.input.pressed = 0;
                msg.body.input.buttons = 0;
                msg.body.input.released = 0;
                msg.body.input.repeat = 0;
                msg.body.input.cameraYaw = yaw;
                msg.body.input.stickX = 0.0f;
                msg.body.input.stickY = 0.0f;
            } else {
                msg.body.input.pressed = pad->pressed;
                msg.body.input.buttons = pad->buttons;
                msg.body.input.released = pad->released;
                msg.body.input.repeat = pad->repeat;
                msg.body.input.cameraYaw = yaw;
                msg.body.input.stickX = pad->stickX;
                msg.body.input.stickY = pad->stickY;
            }
            if (NetCharaPushMessage(chara, (u32 *)&msg)) {
                PadRead(self->remotePad, 0);
            }
        } else {
            NetPlayExchangeEntries(self);
        }
        if (!self->synced && NetModeFlagIsClear()) {
            NetCharaSetReady(chara);
        }
        NetCharaCommitFrame(chara);
    }
    if (CoreTaskFind(0x15e) != NULL) {
        NetPlayRequestAbort(self);
    }
    if (self->state == 7 && IoDiscHasManager() && IoDiscIsBusy(IoDiscGetManager())) {
        if (IoUmdIsMediaReady()) {
            self->umdWaitFrames = 0;
        } else {
            self->umdWaitFrames++;
            if (self->umdWaitFrames >= 600) {
                if (SaveHasProfile()) {
                    SaveProfileSetFlags(SaveGetProfile(), 0x80);
                }
                NetPlayRequestAbort(self);
            }
        }
    }
}
