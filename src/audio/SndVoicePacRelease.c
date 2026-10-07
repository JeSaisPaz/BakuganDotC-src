// bdc 0x089c2eb0 SndVoicePacRelease
#include "bdc.h"

/* Releases the loaded voice package: if the package loader (`g_ioPacLoader`) exists and is in
   state 2 (loaded), `IoDataMngReleaseOwner(mgr, loader->entry)` detaches the pac entry's owner tag
   from every file it registered, the loader moves to state 3 (releasing) and the function returns
   1; in any other state (or without a loader) it returns 0 and does nothing. The loader drops back
   to idle in later frames, see `SndVoicePacIsIdle`. */

bool SndVoicePacRelease(void)
{
  if (!IoHasPacLoader()) {
    return false;
  }
  IoPacLoader *loader = IoGetPacLoader();
  return IoPacLoaderRelease(loader);
}
