// bdc 0x089c7240 SndManagerInit
#include "bdc.h"

/* Constructor of the `SndManager` (0x8be8 bytes, allocated by `SndInit`). It sets the two block
   sizes (`framesPerBlock = 0x100`, `blockBytes = 0x400`), creates the two locks `"COSound_Bgm"`
   (`bgmLock`) and `"COSound_Se"` (`lock`, the lock all queue/slot functions take) as LwMutexes
   (`CoreLockInit`, each allocated from the low heap; NULL if the allocation fails), sets
   `state = 1`, clears `volatileBlock`, `suspended`, `buffersReady`, resets the key-on defaults of
   the audio settings block `g_soundAudioSettings` (`masterVolume = 0x7f`, `pan = 0x40`,
   `masterVolume2 = 0x7f`, `pan2 = 0x40`, `unk08 = 0`), clears `resetRequest` and the command
   queue handles and sets `handleCounter = 0x10000`, then stores a freshly allocated 0x100-byte
   bank table in `g_soundAudioSettings->bankTable`. For each of the 32 group slots it sets
   `bankId = -1`, allocates a bank object (`IoLzsPackageCtor`, unlinked with `CoreNodeUnlink`,
   `ownsDir = 0`), clears `state`, `dataSize`, `fileReq[]`, `isPackage`, `loaded`, `name` and sets
   `data = 1`, `groupId = -1`. It zeroes the 32 voice slots (`handle = 0`, `voiceId = -1`), sets the
   master/BGM/voice volume factors (`SndManagerSetMasterVolume` 1.0, `SndManagerSetBgmVolume`
   0.7, `SndManagerSetVoiceVolume` 1.0), initialises the five category volumes to 1.0, clears
   the settings words `unk210..unk218` and `stopFlag` / `flag8be5`. Returns `mgr`. */

SndManager *SndManagerInit(SndManager *mgr)

{
  bool fromLow;
  CoreLock *lock;
  void **table;
  IoLzsPackage *bank;
  SndGroupSlot *group;
  s32 i;

  mgr->framesPerBlock = 0x100;
  mgr->blockBytes = 0x400;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  lock = MemAlloc(0x38, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (lock != NULL) {
    CoreLockInit(lock, "COSound_Bgm", CORE_LOCK_LWMUTEX);
  }
  mgr->bgmLock = lock;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  lock = MemAlloc(0x38, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (lock != NULL) {
    CoreLockInit(lock, "COSound_Se", CORE_LOCK_LWMUTEX);
  }
  mgr->lock = lock;

  mgr->state = 1;
  mgr->volatileBlock = NULL;
  mgr->suspended = 0;
  mgr->buffersReady = 0;
  g_soundAudioSettings->masterVolume = 0x7f;
  g_soundAudioSettings->pan = 0x40;
  g_soundAudioSettings->masterVolume2 = 0x7f;
  g_soundAudioSettings->pan2 = 0x40;
  g_soundAudioSettings->unk08 = 0;
  mgr->resetRequest = 0;
  mgr->handleCounter = 0x10000;
  for (i = 0; i < 0x20; i++) {
    mgr->cmdHandle[i] = 0;
  }

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  table = MemAlloc(0x100, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_soundAudioSettings->bankTable = table;

  for (i = 0; i < 0x20; i++) {
    group = &mgr->groups[i];
    group->bankId = -1;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    bank = MemAlloc(0x44, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (bank != NULL) {
      IoLzsPackageCtor(bank);
    }
    group->bank = bank;
    CoreNodeUnlink((CoreNode *)bank);
    ((IoLzsPackage *)group->bank)->ownsDir = 0;
    group->state = 0;
    group->data = (void *)(uintptr_t)1;
    group->dataSize = 0;
    group->fileReq[0] = NULL;
    group->fileReq[1] = NULL;
    group->fileReq[2] = NULL;
    group->isPackage = 0;
    group->loaded = 0;
    group->name = NULL;
    group->groupId = -1;
  }

  memset(mgr->voices, 0, sizeof(mgr->voices));
  for (i = 0; i < 0x20; i++) {
    mgr->voices[i].handle = 0;
    mgr->voices[i].voiceId = -1;
  }

  SndManagerSetMasterVolume(1.0f, mgr);
  SndManagerSetBgmVolume(0.7f, mgr);
  SndManagerSetVoiceVolume(1.0f, mgr);
  for (i = 0; i < 5; i++) {
    mgr->catVolume[i][1] = 1.0f;
    mgr->catVolume[i][0] = 1.0f;
  }

  g_soundAudioSettings->unk210 = 0;
  g_soundAudioSettings->unk214 = 0;
  g_soundAudioSettings->unk218 = 0;
  mgr->stopFlag = 0;
  mgr->flag8be5 = 0;
  return mgr;
}
