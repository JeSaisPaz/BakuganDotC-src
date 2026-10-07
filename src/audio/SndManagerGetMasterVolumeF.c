// bdc 0x089c5c38 SndManagerGetMasterVolumeF
#include "bdc.h"

/* Returns the float master volume stored by `SndManagerSetMasterVolume` (`SndManager + 0x8be0`,
   0..1), as opposed to `SndManagerGetMasterVolume`, which returns the 0..127 byte. */

float SndManagerGetMasterVolumeF(SndManager *mgr)

{
  return mgr->masterVolume;
}

