// bdc 0x088cd350 GameDebugStageSelectDraw
#include "bdc.h"

/* Draw method (vtable slot 4) of the developer stage-select menu (id 501): resets the outline
   colour of every printer to `g_colorWhite`, prints with printer 2 the labels `Stage` / `Area`
   and the help lines `Plus  : Select category and value.` / `Start : Start adventure.`, with
   printer 0 the current region name (`g_debugStageRegionNames`) and with printer 1 the stage
   number (`stage + 1`); the printer of the selected row (`cursor`) gets the outline alpha
   `highlight`, then each of the three printers is drawn through its layer's vtable slot 5 into a
   new sort-key-50 render packet (`GfxNewRenderPacket`). */

void GameDebugStageSelectDraw(GameDebugStageSelect *self)

{
  UiTextPrinter *printer;
  const VtblEntry *entry;
  short delta;
  void *packet;
  int i;
  
  printer = self->printers[2];
  printer->outlineColor[0] = g_colorWhite.x;
  printer->outlineColor[1] = g_colorWhite.y;
  printer->outlineColor[2] = g_colorWhite.z;
  printer->outlineColor[3] = g_colorWhite.w;
  UiTextPrinterPrintf(25.0f,25.0f,self->printers[2],"Stage");
  UiTextPrinterPrintf(25.0f,38.0f,self->printers[2],"Area");
  UiTextPrinterPrintf(25.0f,64.0f,self->printers[2],"Plus  : Select category and value.");
  UiTextPrinterPrintf(25.0f,77.0f,self->printers[2],"Start : Start adventure.");
  UiTextPrinterPrintf(65.0f,25.0f,self->printers[0],"%s",g_debugStageRegionNames[self->region]);
  UiTextPrinterPrintf(65.0f,38.0f,self->printers[1],"%d",self->stage + 1);
  for (i = 0; i < 3; i++) {
    printer = self->printers[i];
    printer->outlineColor[0] = g_colorWhite.x;
    printer->outlineColor[1] = g_colorWhite.y;
    printer->outlineColor[2] = g_colorWhite.z;
    printer->outlineColor[3] = g_colorWhite.w;
    if (i == self->cursor) {
      self->printers[i]->outlineColor[3] = self->highlight;
    }
    printer = self->printers[i];
    entry = &printer->layer.vtbl[5];
    delta = entry->delta;
    packet = GfxNewRenderPacket(50.0f);
    ((void (*)(void *,void *))entry->fn)((u8 *)&printer->layer + delta,packet);
  }
  return;
}
