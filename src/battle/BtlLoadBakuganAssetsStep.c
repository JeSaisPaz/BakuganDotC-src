// bdc 0x0886fce4 BtlLoadBakuganAssetsStep
#include "bdc.h"

/* One step of the asset-loading state machine for unit kind `kind` (step in
   `g_btlBakuganLoadStep`); returns 1 when everything is loaded. Steps: 0 load the model archive
   (`BtlBuildBakuganModelPath`) into a lazily created `IoLzsPackageCtor` package `*modelPkg`
   unless already resident (`IoDataMngFindByPath`); 1 poll it; 2 load the motion archive
   (`BtlBuildBakuganMotionPath`) into `*motionPkg` unless the kind's first motion name
   (`g_charMotionNameLists`) is already known to the motion manager; 3 poll; 4–5 register the
   package as the kind's sound group (id from `g_btlBakuganSoundGroupPairs`, 0 if the kind is not
   listed) and load it (`BtlRegisterSoundPack`, `SndManagerLoadGroup`); 6–7 same for the
   attribute voice group from `BtlGetAttrSoundGroupId`; 8 done. */

int BtlLoadBakuganAssetsStep(int kind, void **modelPkg, void **motionPkg)
{
  int result = 0;
  s32 group = 0;
  s32 attrGroup;
  u32 i;
  char **names;
  bool fromLow;
  IoLzsPackage *pkg;
  IoLzsPackage *alloc;

  for (i = 0; i < 32; i++) {
    if (g_btlBakuganSoundGroupPairs[i][0] == kind) {
      group = g_btlBakuganSoundGroupPairs[i][1];
      break;
    }
  }

  switch (g_btlBakuganLoadStep) {
  case 0:
    BtlBuildBakuganModelPath(kind, g_btlBakuganLoadPath);
    if (IoDataMngFindByPath(IoGetDataMng(), g_btlBakuganLoadPath) != NULL) {
      /* Model already resident: check the motion right away. */
      names = g_charMotionNameLists[kind];
      if (GmoMotionIndexOfName(GmoMotionMgrGet(), names[0]) != -1) {
        g_btlBakuganLoadStep = 0;
        result = 1;
      } else {
        g_btlBakuganLoadStep = 2;
      }
      break;
    }
    pkg = *modelPkg;
    if (pkg == NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      alloc = MemAlloc(sizeof(IoLzsPackage), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      pkg = NULL;
      if (alloc != NULL) {
        IoLzsPackageCtor(alloc);
        pkg = alloc;
      }
      *modelPkg = pkg;
    }
    if (IoLzsPackageStartLoad(pkg, g_btlBakuganLoadPath, 10, 1, 1) != 0) {
      g_btlBakuganLoadStep = 1;
    }
    break;
  case 1:
    if (IoLzsPackagePoll(*modelPkg, 1) == 0) {
      break;
    }
    g_btlBakuganLoadStep = 2;
    /* fallthrough */
  case 2:
    BtlBuildBakuganMotionPath(kind, g_btlBakuganLoadPath);
    names = g_charMotionNameLists[kind];
    if (GmoMotionIndexOfName(GmoMotionMgrGet(), names[0]) != -1) {
      g_btlBakuganLoadStep = 0;
      result = 1;
      break;
    }
    pkg = *motionPkg;
    if (pkg == NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(false);
      alloc = MemAlloc(sizeof(IoLzsPackage), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      pkg = NULL;
      if (alloc != NULL) {
        IoLzsPackageCtor(alloc);
        pkg = alloc;
      }
      *motionPkg = pkg;
    }
    if (IoLzsPackageStartLoad(pkg, g_btlBakuganLoadPath, 10, 0, 1) != 0) {
      g_btlBakuganLoadStep = 3;
    }
    break;
  case 3:
    if (IoLzsPackagePoll(*motionPkg, 0) != 0) {
      g_btlBakuganLoadStep = 4;
    }
    break;
  case 4:
    if (BtlRegisterSoundPack(*modelPkg, group) == 0) {
      break;
    }
    g_btlBakuganLoadStep++;
    /* fallthrough */
  case 5:
    if (!SndManagerLoadGroup(SndGetManager(), group)) {
      break;
    }
    if (BtlGetAttrSoundGroupId(SaveGetProfile(), kind) == -1) {
      g_btlBakuganLoadStep = 8;
      break;
    }
    g_btlBakuganLoadStep++;
    /* fallthrough */
  case 6:
    attrGroup = BtlGetAttrSoundGroupId(SaveGetProfile(), kind);
    if (BtlRegisterSoundPack(*modelPkg, attrGroup) == 0) {
      break;
    }
    g_btlBakuganLoadStep++;
    /* fallthrough */
  case 7:
    attrGroup = BtlGetAttrSoundGroupId(SaveGetProfile(), kind);
    if (!SndManagerLoadGroup(SndGetManager(), attrGroup)) {
      break;
    }
    g_btlBakuganLoadStep++;
    /* fallthrough */
  case 8:
    g_btlBakuganLoadStep = 0;
    result = 1;
    break;
  }
  return result;
}
