// bdc 0x089c2f38 SndVoicePacIsIdle
#include "bdc.h"

/* Returns 1 if the voice package loader (`g_ioPacLoader`) exists and is in state 0 (idle: no
   package loaded, loading or being released), else 0. A new `SndVoicePacLoad` is only accepted in
   this state. */

bool SndVoicePacIsIdle(void)
{
  if (!IoHasPacLoader()) {
    return false;
  }
  void *loader = IoGetPacLoader();
  return IoPacLoaderIsIdle(loader);
}
