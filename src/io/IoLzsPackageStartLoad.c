// bdc 0x089bda48 IoLzsPackageStartLoad
#include "bdc.h"

/* Starts an asynchronous read of `path` through the data manager: clears `owner` (+0x30); when no
   request exists yet and the data manager exists (`IoDataMngExists`), requests the file with
   `IoDataMngRequest` (owner slot `&self->request`, buffer `flag`, copy-path `flag2`) and stores the
   handle at `request` (+0x2c), then ORs `prio` into its request flags (`IoDataAddFlags`) and clears
   `flag29`, `registered` and `ownsDir`. Returns 1 when a request already existed or was created,
   0 when the data manager is missing or the request failed (then `request` is NULL). */

int IoLzsPackageStartLoad(IoLzsPackage *self, const char *path, int prio, u8 flag, u8 flag2)
{
    IoData *req;

    self->owner = NULL;
    if (self->request != NULL) {
        return 1;
    }
    if (IoDataMngExists()) {
        req = IoDataMngRequest(IoGetDataMng(), &self->request, (char *)path, flag, false, flag2);
        self->request = req;
    } else {
        self->request = NULL;
        req = NULL;
    }
    if (req == NULL) {
        return 0;
    }
    IoDataAddFlags(req, prio);
    self->flag29 = 0;
    self->registered = 0;
    self->ownsDir = 0;
    return 1;
}
