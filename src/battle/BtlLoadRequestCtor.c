// bdc 0x0885b618 BtlLoadRequestCtor
#include "bdc.h"

/* Constructor of a `BtlLoadRequest`: `CoreObjectInit` (no chain), installs
   `g_btlLoadRequestVtbl`, clears the four package slots and the step, stores `kind` and clears
   the loading/done bytes. Returns `req`. */
BtlLoadRequest *BtlLoadRequestCtor(BtlLoadRequest *req, int kind)
{
    CoreObjectInit(&req->base, NULL);
    req->base.vtable = g_btlLoadRequestVtbl;
    req->packages[0] = NULL;
    req->packages[1] = NULL;
    req->packages[2] = NULL;
    req->packages[3] = NULL;
    req->step = 0;
    req->kind = kind;
    req->loading = 0;
    req->done = 0;
    return req;
}
