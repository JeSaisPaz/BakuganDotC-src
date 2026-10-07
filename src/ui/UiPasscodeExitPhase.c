// bdc 0x0893dbb0 UiPasscodeExitPhase
#include "bdc.h"

/* Exit phase (entry 3 of the phase table `0x08a9ce20`) of the sequence-code screen (task 374,
   `UiPasscodeCtor`; the player re-enters a sequence of up to 6 symbols from a 10-symbol pad and
   it is compared with the answer): sets `closeRequested` (`+0x4c`) so `UiScreenUpdateCommon`
   removes the task. */

void UiPasscodeExitPhase(UiScreen *screen)

{
  screen->closeRequested = '\x01';
  return;
}

