// bdc 0x0884c900 BtlLoadSoundGroupFromPac
#include "bdc.h"

/* When the sound manager and the PAC loader both exist, registers the entries of the in-memory pack
   `pkg` whose names contain the file name of sound-effect group `groupId` (`SndGetGroupName`)
   with the PAC loader (`IoPacLoaderRegisterPack`); returns that call's result (1 on success), or
   0 when either singleton is missing. `main` is unused. */

int BtlLoadSoundGroupFromPac(void *main, void *pkg, s32 groupId)
{
    void *loader;
    char *groupName;

    (void)main;
    if (!SndHasManager() || !IoHasPacLoader()) {
        return 0;
    }
    loader = IoGetPacLoader();
    groupName = SndGetGroupName(SndGetManager(), groupId);
    return IoPacLoaderRegisterPack(loader, pkg, groupName);
}
