// bdc 0x089bc49c BootNetworkThread
#include "bdc.h"

/* Entry of thread slot 18 "MyThread-Network": creates the ad-hoc manager if needed
   (`NetAdhocCreate`), then runs `NetAdhocUpdate` once per vblank until the manager is gone or
   it reports shutdown (`NetAdhocIsStopped` and `NetAdhocIsThreadExitRequested` both non-zero),
   and destroys it (`NetAdhocDestroy`). Returns 0. */

int BootNetworkThread(void) {
    if (!NetAdhocHasManager()) {
        NetAdhocCreate();
    }
    while (NetAdhocHasManager()) {
        NetAdhocUpdate(NetAdhocGetManager());
        if (NetAdhocIsStopped(NetAdhocGetManager())) {
            NetAdhocGetManager();
            if (NetAdhocIsThreadExitRequested()) {
                sceDisplayWaitVblankStartCB();
                break;
            }
        }
        sceDisplayWaitVblankStartCB();
    }
    NetAdhocDestroy();
    return 0;
}
