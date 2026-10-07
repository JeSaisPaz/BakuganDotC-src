// bdc 0x08a2948c SndSasTerm
#include "bdc.h"

/* Marks the SAS layer uninitialised (`g_sndSasInitialized = 0`); returns `0x80420100` if it was not
   initialised. No SAS call is made (the core needs no teardown). */

int SndSasTerm(void)

{
  int ret;

  ret = 0x80420100;
  if (g_sndSasInitialized == 1) {
    g_sndSasInitialized = 0;
    ret = 0;
  }
  return ret;
}

