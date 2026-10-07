// bdc 0x0890cd20 UiLoadingAnimateIcons
#include "bdc.h"

/* Per-frame icon animation of the now-loading screen (task 10100 / 0x2774, 0x240 bytes, vtable
   `0x08af47dc`, `UiLoadingCtor`; shared UI objects `0x08ac0e80`): steps the shared animation
   player (`0x08ac0e80+0x10`, `GfxFabUpdate`), rocks the ball (`UiLoadingAnimateBall`) and
   handles the icon input (`UiLoadingHandleIconInput`). */

void UiLoadingAnimateIcons(UiLoading *self)

{
  if (g_uiLoadingShared->fab != (GfxFab *)0x0) {
    GfxFabUpdate(g_uiLoadingShared->fab);
  }
  UiLoadingAnimateBall(self);
  UiLoadingHandleIconInput(self);
  return;
}

