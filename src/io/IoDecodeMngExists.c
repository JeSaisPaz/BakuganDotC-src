// bdc 0x089fe130 IoDecodeMngExists
#include "bdc.h"

/* Returns whether the decode manager singleton (`g_ioDecodeMng`, the `CODecodeMng` object created by
   `IoDecodeMngCreate`) exists. */

bool IoDecodeMngExists(void)

{
  return g_ioDecodeMng != (IoDecodeMng *)0x0;
}

