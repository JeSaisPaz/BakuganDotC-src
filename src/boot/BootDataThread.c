// bdc 0x089bc294 BootDataThread
#include "bdc.h"

/* Entry of thread slot 4 "MyThread-Data": initialises the data-load manager (`IoDataMngCreate`),
   then services it (`IoDataMngUpdate(IoGetDataMng())`) for as long as `IoDataMngExists`, and
   tears it down (`IoDataMngDestroy`). Returns 0. */

int BootDataThread(void)
{
    IoDataMngCreate();
    while (IoDataMngExists()) {
        IoDataMngUpdate(IoGetDataMng());
    }
    IoDataMngDestroy();
    return 0;
}
