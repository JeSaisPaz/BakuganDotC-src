// bdc 0x089fce64 IoGetDataMng
#include "bdc.h"

/* Returns the data manager (`CODataMng`, `g_ioDataMng`). */

void *IoGetDataMng(void)

{
  return g_ioDataMng;
}

