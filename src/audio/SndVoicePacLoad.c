// bdc 0x089c2e50 SndVoicePacLoad
#include "bdc.h"

/* Starts loading voice package `index` (a `voice/<LANG>/VO_PAC_*.pac` file,
   `SndBuildVoicePacPath`) through the single package loader `g_ioPacLoader` (an 8-byte `{state,
   entry}` object created by `IoPacLoaderCreate`). Returns 0 when the loader does not exist or is
   busy; otherwise `IoPacLoaderLoad(loader, path)` files a CODataMng request for the `.pac`
   (allocation flag 1, owner = the loader, no re-use check), sets bit 2 of the entry state, stores
   the entry in `loader->entry`, moves the loader to state 1 (loading) and the function returns 1.
    */

bool SndVoicePacLoad(s32 index)
{
  if (!IoHasPacLoader()) {
    return false;
  }
  IoPacLoader *loader = IoGetPacLoader();
  char *path = SndBuildVoicePacPath(index);
  return IoPacLoaderLoad(loader, path);
}
