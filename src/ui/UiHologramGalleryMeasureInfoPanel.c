// bdc 0x0891c698 UiHologramGalleryMeasureInfoPanel
#include "bdc.h"

/* Records the x/y position offsets between pairs of info-panel sprites of the hologram gallery
   screen (`UiHologramGalleryCtor`, task 391) into infoOffset[5]: sprite 162 minus 69, 177 minus
   178, and 170 minus 166 / 65 / 98 (sprites of UiHologramGalleryData). */

void UiHologramGalleryMeasureInfoPanel(UiHologramGallery *self)

{
  UiHologramGalleryData *data = (UiHologramGalleryData *)self->base.data;

  self->infoOffset[0][0] = data->sprites[162]->posX - data->sprites[69]->posX;
  self->infoOffset[0][1] = data->sprites[162]->posY - data->sprites[69]->posY;
  self->infoOffset[1][0] = data->sprites[177]->posX - data->sprites[178]->posX;
  self->infoOffset[1][1] = data->sprites[177]->posY - data->sprites[178]->posY;
  self->infoOffset[2][0] = data->sprites[170]->posX - data->sprites[166]->posX;
  self->infoOffset[2][1] = data->sprites[170]->posY - data->sprites[166]->posY;
  self->infoOffset[3][0] = data->sprites[170]->posX - data->sprites[65]->posX;
  self->infoOffset[3][1] = data->sprites[170]->posY - data->sprites[65]->posY;
  self->infoOffset[4][0] = data->sprites[170]->posX - data->sprites[98]->posX;
  self->infoOffset[4][1] = data->sprites[170]->posY - data->sprites[98]->posY;
}
