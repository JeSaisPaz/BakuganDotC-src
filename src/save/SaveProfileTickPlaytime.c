// bdc 0x0880d660 SaveProfileTickPlaytime
#include "bdc.h"

/* Per-frame play-time clock of the player profile, called by `BootEndOfFrame`: increments
   `g_playtimeFrameCounter` and, when the profile block exists and the counter has reached
   `GfxDisplayGetFps`(`g_gfxDisplay`) frames, adds one second to counter 0 with
   `SaveProfileAddCounter` and resets the frame counter. */

void SaveProfileTickPlaytime(SaveProfile *self)

{
  s32 fps;
  
  g_playtimeFrameCounter = g_playtimeFrameCounter + 1;
  if ((self->data != (SaveProfileData *)0x0) &&
     (fps = GfxDisplayGetFps(g_gfxDisplay), fps <= g_playtimeFrameCounter)) {
    SaveProfileAddCounter(self,0,1);
    g_playtimeFrameCounter = 0;
  }
  return;
}

