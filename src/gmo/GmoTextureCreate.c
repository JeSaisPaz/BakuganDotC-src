// bdc 0x08a1251c GmoTextureCreate
#include "bdc.h"

/* Allocates a 0x40-byte texture record from image-heap pool 0 (`GmoImageHeapAlloc`) and
   initialises it (`GmoTextureInit`). */

void *GmoTextureCreate(void)
{
  return GmoTextureInit(GmoImageHeapAlloc(0, 0x10, 0x40));
}
