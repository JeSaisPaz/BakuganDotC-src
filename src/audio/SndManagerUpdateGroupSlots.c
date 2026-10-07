// bdc 0x089c778c SndManagerUpdateGroupSlots
#include "bdc.h"

/* Per-frame loader state machine of the 32 sound-bank group slots (`SndGroupSlot`, `state` =
   `s16` at slot `+0x420`), run under the manager lock by `SndManagerProcessCommands`. Each slot
   that was set up by `SndManagerSetupGroupSlot` is driven through the asynchronous file loader
   (the data manager behind `IoGetDataMng`) until its bank is registered with the Sony sound
   layer: states 1/2/3 handle a `seGRP_*.pac` package (1 request the file, 2 wait for it, 3 register
   the bank), states 0xb..0xf handle a group that is stored as separate files (0xb request
   `<name>.phd`, 0xc wait + request `<name>.pbd`, 0xd wait + register, 0xe request `<name>.pef`, 0xf
   wait + apply). Returns true when every one of the 32 slots is idle this frame, i.e. its state is
   outside 1..15 (normally 0) or one of the unused states 4..10; false while any slot is loading. */

/* Package path: takes the loaded file of `fileReq[0]` as the slot's data buffer (when `data` is
   still the "not set" value 1), wraps it into the slot's package object, detaches that from its
   lists, records the loaded size and moves on to state 3. */
static void OpenGroupPackage(SndGroupSlot *slot, u8 doRegister)
{
  IoLzsPackage *pkg;

  if (slot->data == (void *)(uintptr_t)1) {
    slot->data = IoDataGetBuffer(slot->fileReq[0]);
  }
  pkg = slot->bank;
  if (pkg != NULL) {
    IoLzsPackageInitWithDir(pkg, slot->data, NULL, doRegister, 1, 1);
  }
  slot->bank = pkg;
  CoreNodeUnlink(&pkg->base);
  ((IoLzsPackage *)slot->bank)->ownsDir = 0;
  slot->dataSize = IoDataGetUserData(slot->fileReq[0]);
  slot->state = 3;
}

/* Clears the slot's sound range, then reads it from the `PPTN` chunk when the `.phd` header and
   the chunk carry their magics. */
static void ReadGroupToneRange(SndGroupSlot *slot, u8 *phd)
{
  SndSsPhdToneChunk *tn;

  slot->soundCount = 0;
  slot->firstSound = 0;
  slot->lastSound = 0;
  if (phd[0] == 'P' && phd[1] == 'P' && phd[2] == 'H' && phd[3] == 'D') {
    tn = (SndSsPhdToneChunk *)(phd + ((SndSsPhdHeader *)phd)->toneOffset);
    if (tn->magic[0] == 'P' && tn->magic[1] == 'P' && tn->magic[2] == 'T' &&
        tn->magic[3] == 'N') {
      slot->soundCount = (s16)((tn->size - 0x18) / tn->entrySize);
      slot->firstSound = (s16)tn->firstTone;
      slot->lastSound = (s16)tn->lastTone;
    }
  }
}

bool SndManagerUpdateGroupSlots(SndManager *mgr)
{
  SndGroupSlot *slot;
  IoData *req;
  void *found;
  void **record;
  u8 *phd;
  void *pbd;
  u8 *pef;
  s32 i;
  s32 idle;
  bool fromTable;
  bool applied;
  bool allIdle;

  allIdle = false;
  idle = 0;
  CoreLockAcquire(mgr->lock);
  slot = mgr->groups;
  for (i = 0; i < 0x20; i++, slot++) {
    switch (slot->state) {
    case 1:
      if (!IoDataMngExists()) {
        break;
      }
      fromTable = false;
      found = IoDataMngFindByPath(IoGetDataMng(), g_soundGroupNames[slot->groupId]);
      if (found != NULL) {
        fromTable = true;
        req = IoDataMngRequest(IoGetDataMng(), slot, g_soundGroupNames[slot->groupId],
                               (u32)(uintptr_t)slot->data, false, false);
        slot->fileReq[0] = req;
      }
      else {
        req = IoDataMngRequest(IoGetDataMng(), slot, slot->name, (u32)(uintptr_t)slot->data, false, false);
        slot->fileReq[0] = req;
      }
      if (req == NULL) {
        break;
      }
      if (!fromTable) {
        IoDataAddFlags(req, 2);
        slot->state = 2;
      }
      else {
        OpenGroupPackage(slot, 0);
      }
      break;
    case 2:
      if (IoDataIsDone(slot->fileReq[0])) {
        OpenGroupPackage(slot, 1);
      }
      break;
    case 3:
      record = &g_soundAudioSettings->bankTable[i * 2];
      phd = CorePackChainFind(slot->bank, "phd");
      pbd = CorePackChainFind(slot->bank, "pbd");
      slot->bankId = SndSsBankRegister(record, phd, pbd);
      if (slot->bankId < 0) {
        break;
      }
      ReadGroupToneRange(slot, CorePackChainFind(slot->bank, "phd"));
      if (i == 0) {
        pef = CorePackChainFind(slot->bank, "pef");
        if (sceSsSetEffectParam((u32 *)pef + 3 /* params after the 0xc-byte head */) < 0) {
          printf("sceSsSetEffectParam() error\n");
        }
      }
      slot->state = 0;
      slot->loaded = 1;
      break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      idle++;
      break;
    case 0xb:
      slot->phdPath[0] = '\0';
      strcpy(slot->phdPath, slot->name);
      strcat(slot->phdPath, ".phd");
      req = IoDataMngRequest(IoGetDataMng(), slot, slot->phdPath, 1, false, false);
      slot->fileReq[0] = req;
      if (req != NULL) {
        IoDataAddFlags(req, 2);
        slot->state++;
      }
      break;
    case 0xc:
      if (!IoDataIsDone(slot->fileReq[0])) {
        break;
      }
      IoDataGetBuffer(slot->fileReq[0]);
      IoDataGetUserData(slot->fileReq[0]);
      slot->pbdPath[0] = '\0';
      strcpy(slot->pbdPath, slot->name);
      strcat(slot->pbdPath, ".pbd");
      req = IoDataMngRequest(IoGetDataMng(), slot, slot->pbdPath, 1, false, false);
      slot->fileReq[1] = req;
      if (req != NULL) {
        IoDataAddFlags(req, 2);
        slot->state++;
      }
      break;
    case 0xd:
      if (!IoDataIsDone(slot->fileReq[1])) {
        break;
      }
      phd = IoDataGetBuffer(slot->fileReq[0]);
      pbd = IoDataGetBuffer(slot->fileReq[1]);
      slot->bankId = SndSsBankRegister(&g_soundAudioSettings->bankTable[i * 2], phd, pbd);
      if (slot->bankId >= 0) {
        ReadGroupToneRange(slot, phd);
        slot->dataSize = IoDataGetUserData(slot->fileReq[0]) + IoDataGetUserData(slot->fileReq[1]);
      }
      if (i != 0) {
        slot->state = 0;
        slot->loaded = 1;
      }
      else {
        slot->state++;
      }
      break;
    case 0xe:
      slot->pefPath[0] = '\0';
      strcpy(slot->pefPath, slot->name);
      strcat(slot->pefPath, ".pef");
      req = IoDataMngRequest(IoGetDataMng(), slot, slot->pefPath, 1, false, false);
      slot->fileReq[2] = req;
      if (req != NULL) {
        IoDataAddFlags(req, 0x22);
        slot->state++;
      }
      break;
    case 0xf:
      if (!IoDataIsDone(slot->fileReq[2])) {
        break;
      }
      applied = false;
      if (IoDataGetLoadedSize(slot->fileReq[2]) == 0) {
        applied = true;
      }
      else {
        pef = IoDataGetBuffer(slot->fileReq[2]);
        if (sceSsSetEffectParam((u32 *)pef + 3 /* params after the 0xc-byte head */) >= 0) {
          applied = true;
        }
        else {
          printf("sceSsSetEffectParam() error\n");
        }
      }
      if (applied) {
        slot->state = 0;
        slot->loaded = 1;
      }
      break;
    default:
      idle++;
      break;
    }
  }
  if (i == idle) {
    allIdle = true;
  }
  CoreLockRelease(mgr->lock);
  return allIdle;
}
