// bdc 0x089c1214 SndHasGroupLoader
#include "bdc.h"

/* Returns 1 if the sound group loader exists: the holder `g_soundGroupLoader` is non-NULL and its
   pointer to the `SndGroupLoader` object is non-zero; else 0. The guard before
   `SndGetGroupLoader` in `SndEmitterUpdateAll`. */

bool SndHasGroupLoader(void)
{
  bool result;

  result = false;
  if ((g_soundGroupLoader != (SndGroupLoader **)0x0) &&
      (*g_soundGroupLoader != (SndGroupLoader *)0x0)) {
    result = true;
  }
  return result;
}
