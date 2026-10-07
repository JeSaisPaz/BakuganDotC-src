// bdc 0x089fd8a4 IoPacLoaderUpdate
#include "bdc.h"

/* Per-frame step of the `.pac` package loader (`g_ioPacLoader`): state 1 waits for the package
   file request (`entry`) to finish (`IoDataIsDone`), then wraps the loaded data in a temporary
   `IoLzsPackage` directory (`IoLzsPackageAttachDir`), sets state 0 and
   registers every entry with the data manager (`IoPacLoaderRegisterPack`, which sets state 2;
   back to state 1 on failure); state 3 waits until no request is owned by the package any more,
   releases the loader's own references, clears `entry` and goes to 4; state 4 waits until no
   request is owned by the loader and returns to idle (0). Other states do nothing. */

void IoPacLoaderUpdate(IoPacLoader *loader)
{
    IoLzsPackage pkg;
    u32 state;

    state = (u32)loader->state;
    if (state >= 5) {
        return;
    }
    if (state == 1) {
        if (loader->entry != NULL && IoDataIsDone(loader->entry)) {
            IoLzsPackageCtor(&pkg);
            IoLzsPackageAttachDir(&pkg, IoDataGetBuffer(loader->entry), 0, 1, 1);
            loader->state = 0;
            if (IoPacLoaderRegisterPack(loader, &pkg, NULL) == 0) {
                loader->state = 1;
            }
            IoLzsPackageDtor(&pkg, 2);
        }
    } else if (state == 3) {
        if (IoDataMngFindByOwner(IoGetDataMng(), loader->entry, NULL) == NULL) {
            IoDataMngReleaseOwner(IoGetDataMng(), loader);
            loader->entry = NULL;
            loader->state = 4;
        }
    } else if (state == 4) {
        if (IoDataMngFindByOwner(IoGetDataMng(), loader, NULL) == NULL) {
            loader->state = 0;
        }
    }
}
