// bdc 0x08a23ae4 SndSsPanToVolumes
#include "bdc.h"

/* Computes SAS left/right volumes from a base level, a 0..0x7f volume and two pans (combined as
   `tonePan + pan - 0x40`, clamped 0..0x7f; 0x40 = centre): the far side is attenuated linearly,
   then both sides are scaled by `volume / 0x7f`. */

void SndSsPanToVolumes(u32 base, s32 volume, s32 tonePan, s32 pan, u32 *left, u32 *right)

{
  s32 pv;
  
  pv = tonePan + pan + -0x40;
  if (pv < 0) {
    pv = 0;
  }
  if (0x7f < pv) {
    pv = 0x7f;
  }
  if (pv == 0) {
    *left = base;
    *right = 0;
  }
  else if (pv < 0x40) {
    *left = base;
    *right = base * pv >> 6;
  }
  else {
    if (pv < 0x41) {
      *left = base;
    }
    else {
      *left = base * (0x7f - pv) >> 6;
    }
    *right = base;
  }
  *left = (*left * volume) / 0x7f;
  *right = (*right * volume) / 0x7f;
  return;
}

