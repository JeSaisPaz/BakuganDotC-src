// bdc 0x0886f594 BtlBuildBakuganMotionPath
#include "bdc.h"

/* Builds the motion archive path of unit kind `kind` into `out`: the directory
   `g_btlBakuganDirPath` (`"data/battle/bakugan/"`) for kinds 1..20 or `g_btlTrapBakuganDirPath`
   (`"data/battle/trapbakugan/"`) otherwise, then the kind's base name from `g_btlUnitKindNames`
   (e.g. `"NeoDragonoid"`), then `g_btlMotionArchiveSuffix` (`"_mot.lzs"`). Called by
   `BtlLoadBakuganAssetsStep`. */
void BtlBuildBakuganMotionPath(int kind, char *out)
{
    if (kind < 1 || kind > 20) {
        strcpy(out, g_btlTrapBakuganDirPath);
    } else {
        strcpy(out, g_btlBakuganDirPath);
    }
    strcat(out, g_btlUnitKindNames[kind]);
    strcat(out, g_btlMotionArchiveSuffix);
}
