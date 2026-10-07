// bdc 0x089fce48 IoDataMngExists
#include "bdc.h"

/* Returns whether the data manager singleton (`g_ioDataMng`, the `CODataMng` object created by
   `IoDataMngCreate` on its own worker thread) exists. */

bool IoDataMngExists(void)

{
  return g_ioDataMng != (IoDataMng *)0x0;
}

