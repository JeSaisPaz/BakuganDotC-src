// bdc 0x089ce184 PadRead
#include "bdc.h"

/* Polls the controller once and updates `pad`: shifts the current buttons into `prevButtons`, reads
   one `SceCtrlData` with `sceCtrlReadBufferPositive` (`blocking != 0`, waits for the next
   sample) or `sceCtrlPeekBufferPositive`, then derives `buttons`, `pressed`, `released`, the
   auto-repeat mask `repeat`, per-button hold counters and the normalised stick `stickX`/`stickY`.
   If the pad is not enabled it only calls `PadInit` and returns. */

void PadRead(PadState *pad, u8 blocking)
{
  SceCtrlData data[8];
  s32 count;
  s32 sx;
  s32 sy;
  u16 cur;
  u16 mask;
  u16 delay;
  u16 hold;
  s32 i;

  if (pad->enabled == 0) {
    PadInit(pad, 0);
    return;
  }
  pad->prevButtons = pad->buttons;
  sx = 0;
  sy = 0;
  if (blocking != 0) {
    count = sceCtrlReadBufferPositive(data, 8);
  }
  else {
    count = sceCtrlPeekBufferPositive(data, 8);
  }
  if (count == 0) {
    pad->readOk = 0;
    cur = pad->buttons;
  }
  else {
    pad->buttons = (u16)data[0].buttons;
    if ((data[0].buttons & 0x20000) == 0) { /* PSP_CTRL_HOLD */
      if (pad->dpadEmulatesStick != 0) {
        u8 dpad = (u8)pad->buttons;
        if ((dpad & 0x80) != 0) { /* left */
          sx = -127;
        }
        if ((dpad & 0x20) != 0) { /* right */
          sx += 127;
        }
        if ((dpad & 0x10) != 0) { /* up */
          sy = -127;
        }
        if ((dpad & 0x40) != 0) { /* down */
          sy += 127;
        }
      }
      if ((sx | sy) == 0) {
        sx = data[0].aX - 0x80;
        sy = data[0].aY - 0x80;
      }
      else if (sx != 0 && sy != 0) {
        /* diagonal: scale both axes by 89/128 (~1/sqrt(2)) */
        sx = (sx * 0x59) / 128;
        sy = (sy * 0x59) / 128;
      }
      pad->stickX = (float)sx * -0.0078125f;
      pad->stickY = (float)sy * 0.0078125f;
    }
    pad->readOk = 1;
    cur = pad->buttons;
  }
  if (pad->stickEmulatesDpad != 0 && (cur & 0xf0) == 0) {
    pad->buttons = (pad->buttons & ~0x80) | ((!(pad->stickX <= 0.8f)) << 7);
    pad->buttons = (pad->buttons & ~0x20) | ((pad->stickX < -0.8f) << 5);
    pad->buttons = (pad->buttons & ~0x10) | ((pad->stickY < -0.8f) << 4);
    pad->buttons = (pad->buttons & ~0x40) | ((!(pad->stickY <= 0.8f)) << 6);
    cur = pad->buttons;
  }
  pad->pressed = ~pad->prevButtons & cur;
  pad->released = pad->prevButtons & ~pad->buttons;
  pad->repeat = 0;
  for (i = 0; i < 17; i++) {
    mask = (u16)(1 << i);
    delay = pad->repeatDelay;
    hold = 0;
    if ((pad->buttons & mask) != 0) {
      hold = pad->holdCount[i] + 1;
    }
    pad->holdCount[i] = hold;
    if (delay < pad->holdCount[i]) {
      pad->holdCount[i] = delay - pad->repeatInterval;
      pad->repeat |= mask;
    }
  }
  pad->repeat |= pad->pressed;
}
