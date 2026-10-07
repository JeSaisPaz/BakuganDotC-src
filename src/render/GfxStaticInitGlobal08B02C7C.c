// bdc 0x089ff728 GfxStaticInitGlobal08B02C7C
#include "bdc.h"

/* Static initialiser (entry 48 of the static-constructor table `0x08af5bbc`, run by
   `CxxRunStaticCtors`) that copies the constant word 14 at `0x08a3a5d0` into the global
   `0x08b02c7c`. */

void GfxStaticInitGlobal08B02C7C(void)

{
  g_gfxStaticWord08B02C7C = g_gfxConstWord14;
  return;
}

