// bdc 0x089bc3a4 BootDevModuleThread
#include "bdc.h"

/* Entry of thread slot 1 "MyThread-DevModule": creates the module manager (`CoreModuleMgrCreate`)
   and then loops forever running `CoreModuleUpdate` on `CoreGetModuleMgr` once per vblank
   (`sceDisplayWaitVblankStartCB`). */

void BootDevModuleThread(void)

{
  CoreModuleMgr *mgr;
  
  CoreModuleMgrCreate();
  do {
    mgr = CoreGetModuleMgr();
    CoreModuleUpdate(mgr);
    sceDisplayWaitVblankStartCB();
  } while( true );
}

