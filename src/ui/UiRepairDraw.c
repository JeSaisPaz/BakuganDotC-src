// bdc 0x0890f7fc UiRepairDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the card repair screen (task id 430): submits the sprite layer in a
   depth-50 render packet, then prints the two message lines with the text printer: `message` at
   (240, 160) in yellow at scale 1 and `detail` at (116, 204) in white at scale 0.75, both with the
   screen alpha `+0x2c0`, each printer drawn into a depth-550 render packet (vtable slot 5). */

typedef void (*RepairPrintFn)(float x, float y, float z, void *self, char *text, s32 centre,
                              s32 a2, s32 a3);
typedef void (*RepairDrawFn)(void *self, void *packet);

void UiRepairDraw(UiScreen *screen)
{
  UiRepair *repair = (UiRepair *)screen;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  void *target;
  void *packet;

  packet = GfxNewRenderPacket(50.0f);
  if (screen->spriteLayer != NULL) {
    GfxSpriteLayerDraw(screen->spriteLayer, packet);
  }

  repair->textBox->scale = 1.0f;
  printer = repair->textBox;
  printer->outlineColor[0] = g_colorYellow.x;
  printer->outlineColor[1] = g_colorYellow.y;
  printer->outlineColor[2] = g_colorYellow.z;
  printer->outlineColor[3] = g_colorYellow.w;
  repair->textBox->outlineColor[3] = repair->alpha;
  printer = repair->textBox;
  entry = &printer->layer.vtbl[2];
  ((RepairPrintFn)entry->fn)(240.0f, 160.0f, 0.0f, (u8 *)printer + entry->delta, repair->message,
                             1, 0, 0);
  printer = repair->textBox;
  entry = &printer->layer.vtbl[5];
  target = (u8 *)printer + entry->delta;
  packet = GfxNewRenderPacket(550.0f);
  ((RepairDrawFn)entry->fn)(target, packet);

  repair->textBox->scale = 0.75f;
  printer = repair->textBox;
  printer->outlineColor[0] = g_colorWhite.x;
  printer->outlineColor[1] = g_colorWhite.y;
  printer->outlineColor[2] = g_colorWhite.z;
  printer->outlineColor[3] = g_colorWhite.w;
  repair->textBox->outlineColor[3] = repair->alpha;
  printer = repair->textBox;
  entry = &printer->layer.vtbl[2];
  ((RepairPrintFn)entry->fn)(116.0f, 204.0f, 0.0f, (u8 *)printer + entry->delta, repair->detail,
                             0, 0, 0);
  printer = repair->textBox;
  entry = &printer->layer.vtbl[5];
  target = (u8 *)printer + entry->delta;
  packet = GfxNewRenderPacket(550.0f);
  ((RepairDrawFn)entry->fn)(target, packet);
}
