// bdc 0x089c2ef4 SndVoicePacIsLoaded
#include "bdc.h"

/* Returns 1 if the voice package loader (`g_ioPacLoader`) exists and is in state 2 (the `.pac` is
   loaded and its files are registered), else 0. */

bool SndVoicePacIsLoaded(void)
{
  if (!IoHasPacLoader()) {
    return false;
  }
  void *loader = IoGetPacLoader();
  return IoPacLoaderIsLoaded(loader);
}
