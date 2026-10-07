// bdc 0x0885b4f4 BtlLoadRequestsUpdate
#include "bdc.h"

/* Walks every pending load request in `g_btlLoadRequests`: steps each one
   (`BtlLoadRequestStep`) unless another request is loading (`BtlLoadRequestOtherBusy`), then
   reads its `done` flag. Returns true when every request is done (also when the list is missing or
   empty). */
bool BtlLoadRequestsUpdate(void)
{
    BtlLoadRequest *req;
    bool allDone = true;

    if (g_btlLoadRequests == NULL) {
        return allDone;
    }
    for (req = (BtlLoadRequest *)g_btlLoadRequests->head; req != NULL;
         req = (BtlLoadRequest *)req->base.next) {
        if (!BtlLoadRequestOtherBusy(req)) {
            BtlLoadRequestStep(req);
        }
        if (req->done == 0) {
            allDone = false;
        }
    }
    return allDone;
}
