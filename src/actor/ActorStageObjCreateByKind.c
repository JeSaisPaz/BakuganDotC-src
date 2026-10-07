// bdc 0x088ace8c ActorStageObjCreateByKind
#include "bdc.h"

/* Factory of stage objects by `kind` (index into `g_actorStageObjModelTable`, 3 entries per kind).
   Returns NULL for kind 0, when `ActorStageObjCanCreate` fails or the kind has no model name.
   Applies hard-coded position fixes (kind 0x2d: -38 in Y plus per-X tweaks on stages 4 and 7; kind
   0x7c: -120 in Y), bumps `g_actorStageObjCreateCount`, then picks the class from the kind's
   category (`ActorStageObjGetCategory`). Categories other than 9 and 12..14 need ground below
   `pos` (`ActorStageObjHasGroundBelow`, waived on stages < 8 or >= 12) and the model in the pack
   chain (`CorePackChainFindData`), else NULL. Categories 12/13 under rule mode 2 need profile
   word 7 == 1. Builds: 3/7 0x380-byte `ActorStageObjPropCtor` (NULL for kind 0xb2 in rule mode 2,
   or on every even creation count when `SaveGetProfileFlag0` is set); 9 kinds 0xa9..0xaf 0x3a0-byte
   `ActorStageObjCrystalCtor`, 0xb0 0x390-byte `ActorStageObjEggCrystalCtor`, 0xb1
   `ActorStageObjLandmarkCreate`, other kinds NULL; 10 `ActorStageObjScreenCtor`; 12
   `ActorStageObjMineCtor`; 13 `ActorStageObjWindGeneratorCtor`; 14 NULL; everything else a
   plain 800-byte `ActorStageObjBaseCtor`. Objects come from the low heap; returns the object or
   NULL when the allocation failed. */

void *ActorStageObjCreateByKind(s32 kind, float *pos)
{
  int category;
  int stage;
  bool groundOptional;
  bool wasLow;
  const char **model;
  void *obj;
  int variant;
  int i;
  float groundPos[4];
  float ctorPos[4];
  float posCopy[4];

  if (kind == 0) {
    return NULL;
  }
  category = ActorStageObjGetCategory(kind);
  stage = g_scriptGlobalVars[1];
  if (stage < 8) {
    groundOptional = true;
  } else if (stage < 12) {
    groundOptional = false;
  } else {
    groundOptional = true;
  }

  if (kind < 0x2e) {
    if (kind >= 0x2d) {
      pos[1] = pos[1] - 38.0f;
      if (g_scriptGlobalVars[1] == 4) {
        if (pos[0] == 1467.0f) {
          pos[0] = pos[0] - 3.0f;
          pos[3] = pos[3] - 1.2f;
        } else if (pos[0] == 1389.0f) {
          pos[3] = pos[3] + 0.5f;
        } else if (pos[0] == 1325.0f) {
          pos[3] = pos[3] - 1.0f;
          pos[1] = pos[1] + 2.3f;
        } else if (pos[0] == 1277.0f) {
          pos[0] = pos[0] - 2.0f;
        }
      } else if (g_scriptGlobalVars[1] == 7) {
        if (pos[0] == 1453.0f) {
          pos[0] = pos[0] + 6.0f;
        } else if (pos[0] == 1364.0f) {
          pos[0] = pos[0] + 6.0f;
          pos[3] = pos[3] - 1.0f;
        } else if (pos[0] == 1301.0f) {
          pos[0] = pos[0] + 6.0f;
          pos[1] = pos[1] + 3.0f;
          pos[3] = pos[3] - 0.5f;
        } else if (pos[0] == 1266.0f) {
          pos[0] = pos[0] + 3.0f;
          pos[1] = pos[1] + 1.0f;
          pos[3] = pos[3] - 1.5f;
        }
      }
    }
  } else if (kind == 0x7c) {
    pos[1] = pos[1] - 120.0f;
  }

  /* Unused copy of pos (sp+0x10). */
  for (i = 0; i < 4; i++) {
    posCopy[i] = pos[i];
  }
  (void)posCopy;
  if (ActorStageObjCanCreate() == 0) {
    return NULL;
  }
  model = &g_actorStageObjModelTable[kind * 3];
  if (*model == NULL) {
    return NULL;
  }
  g_actorStageObjCreateCount = g_actorStageObjCreateCount + 1;

  if (category == 9 || category == 14) {
    /* no ground / pack check */
  } else if (category == 12 || category == 13) {
    if (g_scriptGlobalVars[8] == 2) {
      if (SaveProfileGetWord(SaveGetProfile(), 7) != 1) {
        return NULL;
      }
    }
  } else {
    for (i = 0; i < 4; i++) {
      groundPos[i] = pos[i];
    }
    if ((ActorStageObjHasGroundBelow(groundPos) | groundOptional) == 0) {
      return NULL;
    }
    if (CorePackChainFindData(g_ioLzsPackages, (char *)*model) == NULL) {
      return NULL;
    }
  }

  obj = NULL;
  switch (category) {
  case 3:
  case 7:
    if (g_scriptGlobalVars[8] == 2 && kind == 0xb2) {
      return NULL;
    }
    if (SaveGetProfileFlag0() != 0) {
      if (g_actorStageObjCreateCount % 2 == 0) {
        return NULL;
      }
    }
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    obj = MemAlloc(0x380, NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    if (obj != NULL) {
      ActorStageObjPropCtor((ActorStageObjProp *)obj, kind, pos);
    }
    break;
  case 9:
    variant = kind - 0xa9;
    if ((u32)variant < 9) {
      switch (kind) {
      case 0xb0:
        MemLock();
        wasLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        obj = MemAlloc(0x390, NULL, 0);
        MemSetAllocFromLow(wasLow);
        MemUnlock();
        if (obj != NULL) {
          for (i = 0; i < 4; i++) {
            ctorPos[i] = pos[i];
          }
          ActorStageObjEggCrystalCtor((ActorStageObjEggCrystal *)obj, ctorPos);
        }
        break;
      case 0xb1:
        for (i = 0; i < 4; i++) {
          ctorPos[i] = pos[i];
        }
        obj = ActorStageObjLandmarkCreate(ctorPos);
        break;
      default:
        MemLock();
        wasLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        obj = MemAlloc(0x3a0, NULL, 0);
        MemSetAllocFromLow(wasLow);
        MemUnlock();
        if (obj != NULL) {
          for (i = 0; i < 4; i++) {
            ctorPos[i] = pos[i];
          }
          ActorStageObjCrystalCtor((ActorStageObjCrystal *)obj, ctorPos, variant, 0xffff);
        }
        break;
      }
    }
    break;
  case 10:
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    obj = MemAlloc(0x340, NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    if (obj != NULL) {
      ActorStageObjScreenCtor((ActorStageObjScreen *)obj, kind, pos);
    }
    break;
  case 12:
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    obj = MemAlloc(0x340, NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    if (obj != NULL) {
      ActorStageObjMineCtor((ActorStageObjMine *)obj, kind, pos);
    }
    break;
  case 13:
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    obj = MemAlloc(0x340, NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    if (obj != NULL) {
      ActorStageObjWindGeneratorCtor((ActorStageObjWindGenerator *)obj, kind, pos);
    }
    break;
  case 14:
    break;
  default:
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    obj = MemAlloc(0x320, NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    if (obj != NULL) {
      ActorStageObjBaseCtor((ActorStageObjBase *)obj, kind, pos);
    }
    break;
  }
  return obj;
}
