// bdc 0x089bdc0c IoLzsPackageDtor
#include "bdc.h"

/* Destructor of the `.lzs` package node: reinstalls `g_ioLzsPackageVtbl`, advances the
   loaded-package list head `g_ioLzsPackages` past it and unlinks it (`CoreNodeUnlink`),
   releases its textures if `registered` (`IoPackDirReleaseTextures`, then delete[]s the
   `textures` array, a `CxxVecBlock`), frees `dir` when `ownsDir` (always clears it), releases the data-manager
   request (`IoDataMngRelease` for `owner`, or for `&request` when there is no owner), runs
   `CoreNodeDtor` and frees the node itself when `flags & 1`. */


void IoLzsPackageDtor(IoLzsPackage *self, u32 flags)
{
    int index;
    u16 *dir;

    if (self == NULL) {
        return;
    }
    self->base.vtable = g_ioLzsPackageVtbl;
    if (g_ioLzsPackages == self) {
        g_ioLzsPackages = (IoLzsPackage *)self->base.next;
    }
    CoreNodeUnlink(&self->base);

    if (self->registered) {
        index = 0;
        IoPackDirReleaseTextures(self, self->dir, &index);
        if (self->textures != NULL) {
            CxxVecBlock *block = (CxxVecBlock *)((u8 *)self->textures - __builtin_offsetof(CxxVecBlock, elements));

            MemLock();
            MemFree(block, NULL, 0);
            MemUnlock();
            self->textures = NULL;
        }
    }
    if (self->ownsDir) {
        dir = self->dir;
        if (dir != NULL) {
            MemLock();
            MemFree(dir, NULL, 0);
            MemUnlock();
            self->dir = NULL;
        }
    } else {
        self->dir = NULL;
    }

    if (self->owner != NULL) {
        void *mng = IoGetDataMng();

        IoDataMngRelease(mng, self->owner, self->request);
    } else if (self->request != NULL) {
        void *mng = IoGetDataMng();

        IoDataMngRelease(mng, &self->request, self->request);
    }

    CoreNodeDtor(&self->base, 0);
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
