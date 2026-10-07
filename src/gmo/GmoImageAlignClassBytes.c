// bdc 0x08a101a0 GmoImageAlignClassBytes
#include "bdc.h"

/* Returns the alignment of a plan class: 3 -> 4, 2 -> 0x10, 1 -> 0x40, 0 -> 0x80. */

u32 GmoImageAlignClassBytes(int cls)
{
  u32 bytes = 4;

  if (((cls != 3) && (bytes = 0x10, cls != 2)) && (bytes = 0x40, cls != 1)) {
    bytes = 0x80;
  }
  return bytes;
}
