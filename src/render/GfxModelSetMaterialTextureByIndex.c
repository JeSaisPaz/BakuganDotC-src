// bdc 0x089e01f4 GfxModelSetMaterialTextureByIndex
#include "bdc.h"

/* Binds `texture` to material `index`: stores `texture + 0x60` in the material's texture slot. */

typedef struct {
  u8 _unk0[4];
  void *image;
  u8 _unk8[8];
} TextureSlot;

typedef struct {
  u8 _unk0[0x60];
  u8 payload[1];
} TextureBlock;

void GfxModelSetMaterialTextureByIndex(GfxModel *self, s32 index, void *texture)

{
  GmoMaterial *material;
  TextureSlot *slots;

  material = (GmoMaterial *)GmoModelGetMaterial(self->data, index);
  slots = (TextureSlot *)self->data->textures;
  slots[material->attrs->layerRef & 0x3ff].image = &((TextureBlock *)texture)->payload;
}
