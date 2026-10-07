// bdc 0x089402fc UiScreen390Ctor
#include "bdc.h"

/* Constructor of `UiScreen390` (task id 390, 0x186), the short cutscene played
   when the player uses a stage terminal: `ActorPlayerStateUseTerminal` opens it and waits for it
   to close. Base `UiScreenCtor`, vtable `g_uiScreen390Vtable`, object size 0x94. Saves the pad's
   `stickEmulatesDpad` and forces it on, clears `unk6c`/`unk70`/`unk78`/`effect`, copies
   the current stage point `g_gameStagePoint` into `stagePoint` and clears the fast-forward flag
   `g_uiScreen390FastForward`. */

UiScreen390 *UiScreen390Ctor(UiScreen390 *screen)

{
  PadState *pad;

  UiScreenCtor(&screen->base.base);
  screen->base.base.vtable = g_uiScreen390Vtable;
  screen->unk6c = 0;
  pad = screen->base.pad;
  screen->unk70 = 0;
  screen->savedStickEmulatesDpad = pad->stickEmulatesDpad;
  pad->stickEmulatesDpad = 1;
  screen->unk78 = 0;
  screen->effect = (GfxEffect *)0;
  screen->stagePoint = g_gameStagePoint;
  g_uiScreen390FastForward = 0;
  return screen;
}
