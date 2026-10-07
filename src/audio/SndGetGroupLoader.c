// bdc 0x089c123c SndGetGroupLoader
#include "bdc.h"

/* Returns the `SndGroupLoader` object (`*g_soundGroupLoader`). Callers check
   `SndHasGroupLoader` first, since the holder is dereferenced unconditionally. */

SndGroupLoader *SndGetGroupLoader(void)

{
  return *g_soundGroupLoader;
}

