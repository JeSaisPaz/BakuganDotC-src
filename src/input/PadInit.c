// bdc 0x089cdfa8 PadInit
#include "bdc.h"

/* Initialises a `PadState`: stores `initArg`, configures the PSP controller
   (`sceCtrlSetSamplingMode`(1), `sceCtrlSetSamplingCycle`(0),
   `sceCtrlSetIdleCancelThreshold`(100, -1)), clears all button masks, hold counters and stick
   values, sets auto-repeat to delay 0x10 / interval 2 frames, enables D-pad-to-stick emulation,
   disables stick-to-D-pad emulation, and clears `buffer` and `unk44`. */

void PadInit(PadState *pad, u8 initArg)
{
  pad->initArg = initArg;
  sceCtrlSetSamplingMode(1);
  sceCtrlSetSamplingCycle(0);
  pad->prevButtons = 0;
  pad->buttons = 0;
  pad->pressed = 0;
  pad->released = 0;
  pad->repeat = 0;
  pad->stickX = 0.0f;
  pad->stickY = 0.0f;
  memset(pad->holdCount, 0, 0x22);
  PadSetRepeatDelay(pad, 0x10);
  PadSetRepeatInterval(pad, 2);
  pad->dpadEmulatesStick = 1;
  pad->stickEmulatesDpad = 0;
  sceCtrlSetIdleCancelThreshold(100, -1);
  pad->buffer = (void *)0;
  pad->unk44 = 0;
}
