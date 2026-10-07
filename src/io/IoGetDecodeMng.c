// bdc 0x089fe14c IoGetDecodeMng
#include "bdc.h"

/* Returns the decode manager singleton `g_ioDecodeMng` (see `IoDecodeMngExists`). */

IoDecodeMng *IoGetDecodeMng(void)

{
  return g_ioDecodeMng;
}

