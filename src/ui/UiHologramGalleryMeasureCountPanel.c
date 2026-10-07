// bdc 0x0891c7e0 UiHologramGalleryMeasureCountPanel
#include "bdc.h"

/* Records the slot-row anchor of the hologram gallery screen (`UiHologramGalleryCtor`, task 391)
   into slotRowOrigin (sprite 104's position, 13 px lower) and the x/y offsets of sprites 108, 116,
   112, 124, 90, 120 and 8 from sprite 103 into slotRowOffset[7] (read by
   UiHologramGalleryTweenSlotPage). */

void UiHologramGalleryMeasureCountPanel(UiHologramGallery *self)

{
  UiHologramGalleryData *data = (UiHologramGalleryData *)self->base.data;

  self->slotRowOrigin[0] = data->sprites[104]->posX;
  self->slotRowOrigin[1] = data->sprites[104]->posY + 13.0f;
  self->slotRowOffset[0][0] = data->sprites[103]->posX - data->sprites[108]->posX;
  self->slotRowOffset[0][1] = data->sprites[103]->posY - data->sprites[108]->posY;
  self->slotRowOffset[1][0] = data->sprites[103]->posX - data->sprites[116]->posX;
  self->slotRowOffset[1][1] = data->sprites[103]->posY - data->sprites[116]->posY;
  self->slotRowOffset[2][0] = data->sprites[103]->posX - data->sprites[112]->posX;
  self->slotRowOffset[2][1] = data->sprites[103]->posY - data->sprites[112]->posY;
  self->slotRowOffset[3][0] = data->sprites[103]->posX - data->sprites[124]->posX;
  self->slotRowOffset[3][1] = data->sprites[103]->posY - data->sprites[124]->posY;
  self->slotRowOffset[4][0] = data->sprites[103]->posX - data->sprites[90]->posX;
  self->slotRowOffset[4][1] = data->sprites[103]->posY - data->sprites[90]->posY;
  self->slotRowOffset[5][0] = data->sprites[103]->posX - data->sprites[120]->posX;
  self->slotRowOffset[5][1] = data->sprites[103]->posY - data->sprites[120]->posY;
  self->slotRowOffset[6][0] = data->sprites[103]->posX - data->sprites[8]->posX;
  self->slotRowOffset[6][1] = data->sprites[103]->posY - data->sprites[8]->posY;
}
