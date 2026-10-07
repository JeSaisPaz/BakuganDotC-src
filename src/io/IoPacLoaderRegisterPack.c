// bdc 0x089fda74 IoPacLoaderRegisterPack
#include "bdc.h"

/* Registers the entries of an in-memory pack `pack` with the data manager on behalf of the `.pac`
   package loader (`g_ioPacLoader`): only while the loader is idle (0) or loaded (2), else returns
   0 (also for a NULL `pack`). The first pack registered becomes the owner (`entry`). For every entry
   (`CorePackGetEntryOffset`/`CorePackGetEntryName`/`CorePackGetEntrySize`) whose name contains
   `filter` (all when NULL) it creates a request whose buffer is the entry data, flags 0x100 (already
   in memory) and user word = entry size; then sets state 2 (loaded) and returns 1. */

int IoPacLoaderRegisterPack(IoPacLoader *loader, void *pack, char *filter)
{
    int index;
    void *data;
    char *name;
    u32 size;
    bool match;
    IoData *req;

    if (pack == NULL) {
        return 0;
    }
    if (loader->state > 0) {
        if (loader->state != 2) {
            return 0;
        }
    } else if (loader->state < 0) {
        return 0;
    }
    if (loader->entry == NULL) {
        loader->entry = pack;
    }
    for (index = 0; (data = CorePackGetEntryOffset(pack, index)) != NULL; index++) {
        name = CorePackGetEntryName(pack, index);
        size = CorePackGetEntrySize(pack, index);
        match = false;
        if (filter == NULL) {
            match = true;
        } else if (strstr(name, filter) != NULL) {
            match = true;
        }
        if (match) {
            req = IoDataMngRequest(IoGetDataMng(), loader->entry, name, (uintptr_t)data, true, false);
            if (req != NULL) {
                IoDataSetFlags(req, 0x100);
                IoDataSetUserData(req, size);
            }
        }
    }
    loader->state = 2;
    return 1;
}
