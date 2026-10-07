// bdc 0x08a28e70 SndSasInit
#include "bdc.h"

/* Initialises the SAS core (`__sceSasInit(&core, grainSize, 32 voices, stereo, 44100 Hz)`) and
   resets all 32 voices to pitch `0x1000`, simple ADSR `(0xf, 0x5fc0)` and zero volume, then marks
   the layer initialised (`g_sndSasInitialized = 1`). Returns `0x80420101` if it already was, or the
   `__sceSasInit` error. */

int SndSasInit(int grainSize)

{
  int i;
  
  i = -0x7fbdfeff;
  if ((g_sndSasInitialized == 0) &&
     (i = __sceSasInit(&g_sndSasCore,grainSize,0x20,PSP_SAS_OUTPUTMODE_STEREO,0xac44),
     i == 0)) {
    i = 0;
    do {
      if (((g_sndSasInitialized != 0) &&
          (__sceSasSetPitch(&g_sndSasCore,i,0x1000), g_sndSasInitialized != 0)) &&
         (__sceSasSetSimpleADSR(&g_sndSasCore,i,0xf,0x5fc0), g_sndSasInitialized != 0)) {
        __sceSasSetVolume(&g_sndSasCore,i,0,0,0,0);
      }
      i = i + 1;
    } while (i < 0x20);
    g_sndSasInitialized = 1;
    i = 0;
  }
  return i;
}

