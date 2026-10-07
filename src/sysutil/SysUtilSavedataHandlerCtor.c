// bdc 0x089cc828 SysUtilSavedataHandlerCtor
#include "bdc.h"

/* Constructor of the savedata dialog handler (kind 1 of `SysUtilOpenDialog`): runs
   `SysUtilHandlerCtor`, installs `g_sysUtilSavedataVtbl`, allocates the zeroed 0x76c-byte
   `SysUtilSavedataBlock` in the low heap and stores it in `g_savedataParams`, then fills it:
   mode LISTLOAD, game name `"ULES01466"`, save name `"BAKUGAN2"`, file `"PLAYDATA.BIN"`, the
   save-name list `"BAKUGAN2001"`..`"BAKUGAN2008"` (`"%s%03d"`), the 16-byte key, the SFO params,
   parental level 5, a low-heap 0x40-byte `msData` block `{gameName, saveName}`, and points
   idList/fileList/sizeInfo at the blocks behind the params. Clears the busy and result words and
   `g_saveRequiredBytes`. Returns `self`. */

SysUtilSavedataHandler *SysUtilSavedataHandlerCtor(SysUtilSavedataHandler *self)
{
  bool fromLow;
  SysUtilSavedataBlock *block;
  SceUtilitySavedataMsDataInfo *msData;
  s32 i;

  SysUtilHandlerCtor(&self->base);
  self->base.vtbl = g_sysUtilSavedataVtbl;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(0x76c, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_savedataParams = block;
  memset(block, 0, 0x76c);

  SysUtilInitDialogCommon((void **)self, g_savedataParams, 0x600);
  g_savedataParams->params.mode = PSP_UTILITY_SAVEDATA_LISTLOAD;
  g_savedataParams->params.multiStatus = 0;
  strcpy(g_savedataParams->params.gameName, "ULES01466");
  strcpy(g_savedataParams->params.saveName, g_savedataSaveNames[0]);
  strcpy(g_savedataParams->params.fileName, g_savedataFileNames[0]);
  for (i = 0; i < 8; i++) {
    sprintf(g_savedataParams->saveNameList[i], "%s%03d", g_savedataSaveNames[0], i + 1);
  }
  g_savedataParams->params.saveNameList = g_savedataParams->saveNameList;
  memcpy(g_savedataParams->params.key, g_savedataKey, 0x10);
  g_savedataParams->params.secureVersion = 0;
  g_savedataParams->params.multiStatus = 0;
  /* The asm passes the pointer value g_gameSfoText itself (not g_gameSfoText[0]) as the source;
     SaveBuildSfoParam then overwrites the title. */
  strcpy(g_savedataParams->params.sfoParam.title, (const char *)g_gameSfoText);
  SaveBuildSfoParam(&g_savedataParams->params.sfoParam);
  g_savedataParams->params.sfoParam.parentalLevel = 5;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  msData = MemAlloc(0x40, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_savedataParams->params.msData = msData;
  memset(g_savedataParams->params.msData, 0, 0x40);
  strcpy(g_savedataParams->params.msData->gameName, g_savedataParams->params.gameName);
  strcpy(g_savedataParams->params.msData->saveName, g_savedataParams->params.saveName);

  g_savedataParams->idListInfo.maxCount = 8;
  g_savedataParams->params.idList = &g_savedataParams->idListInfo;
  g_savedataParams->fileListInfo.maxSecureEntries = 8;
  g_savedataParams->fileListInfo.maxNormalEntries = 0;
  g_savedataParams->fileListInfo.maxSystemEntries = 1;
  g_savedataParams->params.fileList = &g_savedataParams->fileListInfo;
  g_savedataParams->params.sizeInfo = &g_savedataParams->sizeInfo;

  g_savedataParams->params.dataBuf = SaveGetDataBlock();
  g_savedataParams->params.dataBufSize = SaveGetDataBlockSize();
  g_savedataParams->params.dataSize = SaveGetDataBlockSize();
  g_savedataParams->busy = 0;
  g_savedataParams->result = 0;
  g_saveRequiredBytes = 0;
  return self;
}
