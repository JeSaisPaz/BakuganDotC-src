// bdc 0x0886fc5c BtlRegisterSoundPack
#include "bdc.h"

/* Registers the loaded package `pack` with the pac loader, filtered by the name of sound
   group `group` (`IoPacLoaderRegisterPack` with the name from `SndGetGroupName`), when
   both the sound manager and the pac loader exist; returns the loader's result (1 on
   success), or 0 when either is missing. */

int BtlRegisterSoundPack(void *pack, s32 group)
{
    void *loader;
    char *filter;
    int result;

    result = 0;
    if (SndHasManager() && IoHasPacLoader()) {
        loader = IoGetPacLoader();
        filter = SndGetGroupName(SndGetManager(), group);
        result = IoPacLoaderRegisterPack(loader, pack, filter);
    }
    return result;
}
