// bdc 0x089bc2e4 BootDecodeThread
#include "bdc.h"

/* Entry of thread slot 3 "MyThread-Decode": initialises the decode manager (`IoDecodeMngCreate`),
   runs `IoDecodeMngUpdate(IoGetDecodeMng())` while `IoDecodeMngExists`, then shuts it down
   (`IoDecodeMngUnregister`). Returns 0. */

int BootDecodeThread(void)
{
  IoDecodeMngCreate();
  while (IoDecodeMngExists()) {
    IoDecodeMngUpdate(IoGetDecodeMng());
  }
  IoDecodeMngUnregister();
  return 0;
}
