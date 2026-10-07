// bdc 0x0893e418 UiPasscodeResetPlayback
#include "bdc.h"

/* Clears the 4-byte playback state `+0x7e8` of `UiPasscode`. */

void UiPasscodeResetPlayback(UiPasscode *screen)

{
  memset(screen->playback,0,4);
}
