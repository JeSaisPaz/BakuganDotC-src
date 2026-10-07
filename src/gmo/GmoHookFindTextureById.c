// bdc 0x08a13b6c GmoHookFindTextureById
#include "bdc.h"

/* Calls the texture-by-id hook (`0x08afceb0`, `GmoSetTextureIdHooks`) with `id` (tail call) and
   returns its texture; NULL when no hook is installed. */

void *GmoHookFindTextureById(u16 id)

{
  if (g_gmoTextureIdLookup != NULL) {
    return ((void *(*)(u16))g_gmoTextureIdLookup)(id);
  }
  return NULL;
}
