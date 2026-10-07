// bdc 0x088ca8e0 UiTalkBalloonUpdate
#include "bdc.h"

/* Update (vtable slot 2) of the talk balloon (`UiTalkBalloonCtor`): does nothing while task 100
   exists with flag 1 (paused); otherwise animates the speaker portrait
   (`UiTalkBalloonAnimatePortrait`, when `g_uiTalkBalloonPortraitEnabled` is set) and runs one text
   step (`UiTalkBalloonStep`) with `g_padState`. */

void UiTalkBalloonUpdate(UiTalkBalloon *self)
{
  PadState *pad;
  void *task;
  int i;

  pad = g_padState;
  task = CoreTaskFind(100);
  if (task == NULL || !CoreTaskHasFlags(task, 1)) {
    if (g_uiTalkBalloonPortraitEnabled != 0) {
      UiTalkBalloonAnimatePortrait(self);
    }
    i = 0;
    do {
      if (UiTalkBalloonStep(self, pad) != 0) {
        return;
      }
      i++;
    } while (i < 1);
  }
}
