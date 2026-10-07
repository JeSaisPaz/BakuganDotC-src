// bdc 0x088b95ac BtlLoadSpherePackage
#include "bdc.h"

/* Incremental loader of the battle sphere package of unit kind `kind`, stepped by
   g_btlSpherePkgLoadStep. Step 0 builds "data/battle/sphere/<name>.lzs" from g_btlUnitKindNames
   and returns 1 (step reset to 0) when the name is NULL or the data manager already holds a live
   request for that path; otherwise it makes sure `*pkg` holds a package node (0x44 bytes from the
   low heap end, IoLzsPackageCtor) and starts the load (IoLzsPackageStartLoad, priority 10), moving
   to step 1 when it is pending. Step 1 polls the load (IoLzsPackagePoll) and returns 1, resetting
   the step, once it is done. Returns 0 otherwise (also for any step outside 0..1). */

/* Allocates and constructs a package node from the low end of the heap; NULL on failure. */
static IoLzsPackage *NewPackageNode(void)
{
    IoLzsPackage *node = NULL;
    IoLzsPackage *mem;
    bool wasFromLow;

    MemLock();
    wasFromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(IoLzsPackage), NULL, 0);
    MemSetAllocFromLow(wasFromLow);
    MemUnlock();
    if (mem != NULL) {
        IoLzsPackageCtor(mem);
        node = mem;
    }
    return node;
}

u32 BtlLoadSpherePackage(void **pkg, s32 kind)
{
    char path[64];
    IoLzsPackage *node;

    if (g_btlSpherePkgLoadStep < 1) {
        if (g_btlSpherePkgLoadStep < 0) {
            return 0;
        }
        if (g_btlUnitKindNames[kind] == NULL) {
            g_btlSpherePkgLoadStep = 0;
            return 1;
        }
        /* Stored inline as five words, terminator included. */
        __builtin_memcpy(path, "data/battle/sphere/", 20);
        strcat(path, g_btlUnitKindNames[kind]);
        strcat(path, ".lzs");
        if (IoDataMngFindByPath(IoGetDataMng(), path) != NULL) {
            g_btlSpherePkgLoadStep = 0;
            return 1;
        }
        node = *pkg;
        if (node == NULL) {
            node = NewPackageNode();
            *pkg = node;
        }
        if (IoLzsPackageStartLoad(node, path, 10, 1, 1) != 0) {
            g_btlSpherePkgLoadStep = 1;
        }
    } else if (g_btlSpherePkgLoadStep < 2) {
        node = *pkg;
        if (node == NULL) {
            node = NewPackageNode();
            *pkg = node;
        }
        if (IoLzsPackagePoll(node, 1) != 0) {
            g_btlSpherePkgLoadStep = 0;
            return 1;
        }
    }
    return 0;
}
