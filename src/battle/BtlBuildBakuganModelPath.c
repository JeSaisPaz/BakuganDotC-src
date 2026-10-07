// bdc 0x0886f614 BtlBuildBakuganModelPath
#include "bdc.h"

/* Builds the model archive path of unit kind `kind` into `out`: the Bakugan directory for kinds
   1..20, the trap Bakugan directory otherwise, then the kind's base name and `".lzs"`, e.g.
   `data/battle/bakugan/NeoDragonoid.lzs` for kind 1. */
void BtlBuildBakuganModelPath(int kind, char *out)
{
    char *name;

    if (kind < 1 || kind > 20) {
        strcpy(out, g_btlTrapBakuganDirPath);
        name = g_btlUnitKindNames[kind];
    } else {
        strcpy(out, g_btlBakuganDirPath);
        name = g_btlUnitKindNames[kind];
    }
    strcat(out, name);
    strcat(out, g_btlModelArchiveSuffix);
}
