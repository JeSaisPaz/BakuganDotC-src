// bdc 0x089ccadc SysUtilSavedataHandlerRequest
#include "bdc.h"

/* Savedata handler virtual slot `+0x14`: stores `request` (0..14) in `self->request`, prepares the
   `SceUtilitySavedataParam` block `g_savedataParams` for it and, if accepted, sets the block's
   busy word to 1 and enables the handler (`SysUtilHandlerSetEnabled`) so
   `SysUtilSavedataHandlerService` starts the dialog. Returns 1 when a request was set up, 0 for
   a request >= 15 or (request 13) a save slot outside 0..8. */

s32 SysUtilSavedataHandlerRequest(SysUtilSavedataHandler *self, u32 request)
{
  s32 ok;
  s32 slot;
  CorePackDirEntry *entry;
  PspUtilitySavedataListSaveNewData *newData;
  u32 size;

  ok = 1;
  self->request = request;
  if (request >= 15) {
    ok = 0;
    goto done;
  }

  switch (request) {
  case 0: /* list save */
    g_savedataParams->params.mode = PSP_UTILITY_SAVEDATA_LISTSAVE;
    g_savedataParams->params.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST;
    entry = CorePackChainFindEntry(g_ioLzsPackages, "ICON0.PNG");
    if (entry != NULL) {
      g_savedataParams->params.icon0FileData.buf = entry->data;
      size = entry->size;
      g_savedataParams->params.icon0FileData.size = size;
      g_savedataParams->params.icon0FileData.bufSize = size;
      newData = &g_savedataParams->newData;
      memset(newData, 0, 0x14);
      g_savedataParams->params.newData = newData;
      newData->icon0.buf = entry->data;
      size = entry->size;
      newData->title = NULL;
      newData->icon0.size = size;
      newData->icon0.bufSize = size;
    }
    entry = CorePackChainFindEntry(g_ioLzsPackages, "PIC1.PNG");
    if (entry != NULL) {
      g_savedataParams->params.pic1FileData.buf = entry->data;
      size = entry->size;
      g_savedataParams->params.pic1FileData.size = size;
      g_savedataParams->params.pic1FileData.bufSize = size;
    }
    break;

  case 1: /* list load */
    g_savedataParams->params.mode = PSP_UTILITY_SAVEDATA_LISTLOAD;
    g_savedataParams->params.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST;
    break;

  case 2: /* list delete-all */
    g_savedataParams->params.mode = PSP_UTILITY_SAVEDATA_LISTALLDELETE;
    break;

  case 3: /* autoload, starting at the first save-name list entry */
    g_savedataParams->params.mode = PSP_UTILITY_SAVEDATA_AUTOLOAD;
    g_savedataParams->params.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST;
    self->result = -1;
    self->slotIndex = 0;
    strcpy(g_savedataParams->params.saveName, g_savedataParams->saveNameList[0]);
    break;

  case 4: /* size check of the game data */
    g_savedataParams->params.mode = SCE_UTILITY_SAVEDATA_SIZES;
    g_savedataParams->params.msFree = &g_savedataParams->msFreeInfo;
    g_savedataParams->params.utilityData = &g_savedataParams->usedDataInfo;
    strcpy(g_savedataParams->params.fileName, g_savedataFileNames[0]);
    entry = CorePackChainFindEntry(g_ioLzsPackages, "ICON0.PNG");
    if (entry != NULL) {
      g_savedataParams->params.icon0FileData.buf = entry->data;
      size = entry->size;
      g_savedataParams->params.icon0FileData.size = size;
      g_savedataParams->params.icon0FileData.bufSize = size;
    }
    entry = CorePackChainFindEntry(g_ioLzsPackages, "PIC1.PNG");
    if (entry != NULL) {
      g_savedataParams->params.pic1FileData.buf = entry->data;
      size = entry->size;
      g_savedataParams->params.pic1FileData.size = size;
      g_savedataParams->params.pic1FileData.bufSize = size;
    }
    break;

  case 5:
  case 7:
  case 8:
  case 9: /* the "Install" data */
    if (request == 9) {
      g_savedataParams->params.mode = PSP_UTILITY_SAVEDATA_AUTOSAVE;
      g_savedataParams->params.msFree = NULL;
      strcpy(g_savedataParams->params.sfoParam.savedataTitle, g_languageSfoStrings[1]);
      strcpy(g_savedataParams->params.sfoParam.detail, g_languageSfoStrings[2]);
    } else if (request == 8) {
      g_savedataParams->params.mode = PSP_UTILITY_SAVEDATA_AUTOSAVE;
      strcpy(g_savedataParams->params.sfoParam.savedataTitle, g_languageSfoStrings[1]);
      strcpy(g_savedataParams->params.sfoParam.detail, g_languageSfoStrings[2]);
    } else if (request == 7) {
      g_savedataParams->params.mode = SCE_UTILITY_SAVEDATA_AUTODELETE;
    } else if (request == 5) {
      g_savedataParams->params.mode = SCE_UTILITY_SAVEDATA_SIZES;
      g_savedataParams->params.msFree = &g_savedataParams->msFreeInfo;
    }
    memset(g_savedataParams->params.fileName, 0, 0xd);
    strcpy(g_savedataParams->params.saveName, g_savedataInstallSaveName);
    g_savedataParams->params.dataBuf = NULL;
    g_savedataParams->params.dataBufSize = 0;
    g_savedataParams->params.dataSize = 0xe700000;
    if (request == 8) {
      break;
    }
    entry = CorePackChainFindEntry(g_ioLzsPackages, "INST_ICON0.PNG");
    if (entry != NULL) {
      g_savedataParams->params.icon0FileData.buf = entry->data;
      size = entry->size;
      g_savedataParams->params.icon0FileData.size = size;
      g_savedataParams->params.icon0FileData.bufSize = size;
    }
    entry = CorePackChainFindEntry(g_ioLzsPackages, "INST_PIC1.PNG");
    if (entry != NULL) {
      g_savedataParams->params.pic1FileData.buf = entry->data;
      size = entry->size;
      g_savedataParams->params.pic1FileData.size = size;
      g_savedataParams->params.pic1FileData.bufSize = size;
    }
    break;

  case 6: /* size check of LANG/LANGUAGE.BIN */
    g_savedataParams->params.mode = SCE_UTILITY_SAVEDATA_SIZES;
    g_savedataParams->params.msFree = &g_savedataParams->msFreeInfo;
    g_savedataParams->params.utilityData = &g_savedataParams->usedDataInfo;
    strcpy(g_savedataParams->params.fileName, g_savedataFileNames[1]);
    strcpy(g_savedataParams->params.saveName, g_savedataSaveNames[1]);
    g_savedataParams->params.dataBuf = NULL;
    g_savedataParams->params.dataBufSize = 0;
    g_savedataParams->params.dataSize = 4;
    break;

  case 10: /* save the language setting */
    g_savedataParams->params.mode = PSP_UTILITY_SAVEDATA_SAVE;
    g_savedataParams->params.dataBuf = SaveGetProfile()->language;
    g_savedataParams->params.dataBufSize = 4;
    g_savedataParams->params.dataSize = 4;
    strcpy(g_savedataParams->params.fileName, g_savedataFileNames[1]);
    strcpy(g_savedataParams->params.saveName, g_savedataSaveNames[1]);
    SaveFillLanguageSfo(g_savedataParams->params.sfoParam.title,
                        g_savedataParams->params.sfoParam.title);
    break;

  case 11: /* autoload the language setting */
    g_savedataParams->params.mode = PSP_UTILITY_SAVEDATA_AUTOLOAD;
    g_savedataParams->params.dataBuf = SaveGetProfile()->language;
    g_savedataParams->params.dataBufSize = 4;
    g_savedataParams->params.dataSize = 4;
    strcpy(g_savedataParams->params.fileName, g_savedataFileNames[1]);
    strcpy(g_savedataParams->params.saveName, g_savedataSaveNames[1]);
    break;

  case 12: /* size check of RECORD/RECORD.BIN */
    g_savedataParams->params.mode = SCE_UTILITY_SAVEDATA_SIZES;
    g_savedataParams->params.dataBuf = SaveProfileGetRecordArea(SaveGetProfile());
    g_savedataParams->params.dataBufSize = 0x1e4;
    g_savedataParams->params.dataSize = 0x1e4;
    g_savedataParams->params.msFree = &g_savedataParams->msFreeInfo;
    g_savedataParams->params.utilityData = &g_savedataParams->usedDataInfo;
    strcpy(g_savedataParams->params.fileName, g_savedataFileNames[2]);
    strcpy(g_savedataParams->params.saveName, g_savedataSaveNames[2]);
    SaveFillArenaRecordSfo(g_savedataParams->params.sfoParam.title,
                           g_savedataParams->params.sfoParam.title);
    break;

  case 13: /* autosave the game data to the current slot */
    g_savedataParams->params.mode = PSP_UTILITY_SAVEDATA_AUTOSAVE;
    slot = SysUtilGetSaveSlotIndex();
    if (slot < 0 || slot >= 9) {
      ok = 0;
    } else {
      memset(g_savedataParams->params.saveName, 0, 0x14);
      sprintf(g_savedataParams->params.saveName, "%s%03d", g_savedataSaveNames[0], slot + 1);
      strcpy(g_savedataParams->params.msData->saveName, g_savedataParams->params.saveName);
      memcpy(g_savedataParams->saveNameList[slot], g_savedataParams->params.saveName, 0x14);
    }
    entry = CorePackChainFindEntry(g_ioLzsPackages, "ICON0.PNG");
    if (entry != NULL) {
      g_savedataParams->params.icon0FileData.buf = entry->data;
      size = entry->size;
      g_savedataParams->params.icon0FileData.size = size;
      g_savedataParams->params.icon0FileData.bufSize = size;
    }
    entry = CorePackChainFindEntry(g_ioLzsPackages, "PIC1.PNG");
    if (entry != NULL) {
      g_savedataParams->params.pic1FileData.buf = entry->data;
      size = entry->size;
      g_savedataParams->params.pic1FileData.size = size;
      g_savedataParams->params.pic1FileData.bufSize = size;
    }
    break;

  case 14: /* autoload the arena record */
    g_savedataParams->params.mode = PSP_UTILITY_SAVEDATA_AUTOLOAD;
    g_savedataParams->params.dataBuf = SaveProfileGetRecordArea(SaveGetProfile());
    g_savedataParams->params.dataBufSize = 0x1e4;
    g_savedataParams->params.dataSize = 0x1e4;
    strcpy(g_savedataParams->params.fileName, g_savedataFileNames[2]);
    strcpy(g_savedataParams->params.saveName, g_savedataSaveNames[2]);
    SaveFillArenaRecordSfo(g_savedataParams->params.sfoParam.title,
                           g_savedataParams->params.sfoParam.title);
    break;
  }

done:
  if (ok != 0) {
    g_savedataParams->busy = 1;
    SysUtilHandlerSetEnabled(&self->base, 1);
  }
  return ok;
}
