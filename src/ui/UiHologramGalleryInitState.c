// bdc 0x0891bf54 UiHologramGalleryInitState
#include "bdc.h"

/* Initialises the hologram gallery screen (`UiHologramGalleryCtor`, task 391; menu cursor
   `+0x77`, panel `+0x74`): derives the field (`battleId / 4`, `+0x2174`) and area (`battleId % 4`,
   `+0x2175`) from the current battle id (script global 1), loads the area's hologram slot layout
   (`GameStageGetInfo` → `stageInfo`, `GameStageGetData3c` → `boardPos`), picks the menu mode, default
   Bakugan (`+0x2177`, attribute `+0x2178`), menu locks/cursor, first-visit help (`+0x224c`), help
   counters and order, and sets/clears global flag 0x20 from profile word 0x2e. */

void UiHologramGalleryInitState(UiHologramGallery *self)
{
    u16 info[10];
    s16 board[15][2];
    SaveProfile *profile;
    int i;

    memset(&self->menuCursor, 0, 3);
    self->panel = 0;
    self->listMode[0] = 0;
    self->fieldId = (u8)(g_scriptGlobalVars[1] / 4);
    self->areaId = (u8)(g_scriptGlobalVars[1] % 4);
    GameStageGetInfo(info, self->fieldId, self->areaId);
    for (i = 0; i < 10; i++) {
        self->stageInfo[i] = info[i];
    }
    GameStageGetData3c(board, self->fieldId, self->areaId);
    for (i = 0; i < 15; i++) {
        self->boardPos[i][0] = board[i][0];
        self->boardPos[i][1] = board[i][1];
    }
    UiHologramGalleryPickMenuMode(self);
    self->bakugan = (u8)UiHologramGalleryPickDefaultBakugan(self);
    self->attribute = UiBakuganGetAttribute(self->bakugan);
    UiHologramGalleryInitMenuLocks(self);
    UiHologramGalleryFirstEnabledItem(self);
    memset(self->slotPulse, 0, 0x24);
    memset(self->listPulse, 0, 0x30);
    self->firstVisitHelp = (u8)UiHologramGalleryPickFirstHelp(self);
    UiHologramGalleryCountHelpSeen(self);
    UiHologramGalleryInitHelpOrder(self);
    memset(&self->pageAnimOn, 0, 8);
    profile = SaveGetProfile();
    if (SaveProfileGetWord(profile, 0x2e) == 1) {
        CoreBitsetSet(0x20, g_scriptGlobalBits);
    } else {
        CoreBitsetClear(0x20, g_scriptGlobalBits);
    }
}
