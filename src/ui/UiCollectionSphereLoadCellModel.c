// bdc 0x0897b2b4 UiCollectionSphereLoadCellModel
#include "bdc.h"

/* One field of `rotX * m`: r.x * rot0 + r.y * rot1 + r.z * rot2 + r.w * rot3 lane by lane, summed
   left to right, with rot's columns rot0 = (1, 0, 0, 0), rot1 = (0, c, s, 0), rot2 = (0, -s, c, 0),
   rot3 = (0, 0, 0, 1). */
static void UiCollectionSphereLoadRotXField(float *d, const float *r, float c, float s)
{
  float x = r[0];
  float y = r[1];
  float z = r[2];
  float w = r[3];

  d[0] = x * 1.0f + y * 0.0f + z * 0.0f + w * 0.0f;
  d[1] = x * 0.0f + y * c + z * -s + w * 0.0f;
  d[2] = x * 0.0f + y * s + z * c + w * 0.0f;
  d[3] = x * 0.0f + y * 0.0f + z * 0.0f + w * 1.0f;
}

/* Loads the 3D model of grid cell `cell` of the sphere (Bakugan figure) collection screen
   (`UiCollectionSphere`, task 312): builds the GMO file name of entry `id`
   of `category` (`UiCollectionSphereBuildModelName`, e.g. `"01_V_Ingram_N_P.gmo"`), allocates a
   0x140-byte model from the low end of the heap and constructs it (`GfxModelCtor`; a failed
   allocation stores NULL and the following accesses go through it), stores it in `models[cell]`,
   then sets its GMO root matrix to rotY(3.14) and transforms each field by rotX(tilt)
   (`UiCollectionSphereGetModelTilt`; VFPU `vrot`, `vmmul`), scales the first three rows by 0.4
   (also stored in `modelScale[cell]`), clears `pos`, sets the motion speed to 0 (vtable slot 6),
   turns lighting on, sets specular `{0.4, 0.4, 0.4, 1}` power 8 (`GfxModelSetSpecular`), copies
   `pos` into the matrix translation row with w = 1, clears `ambient[3]` and runs
   `UiCollectionSphereModelMaterialCallback` on every material (`GfxModelForEachMaterial`).
   `unused` is not read. */
void UiCollectionSphereLoadCellModel(UiCollectionSphere *self, u8 category, u8 id, u32 unused, u8 cell)
{
  float specular[4];
  char name[72];
  GfxModel *model;
  GfxModel *mem;
  float *m;
  const VtblEntry *entry;
  bool fromLow;
  float tilt;
  float c;
  float sn;
  int i;

  UiCollectionSphereBuildModelName(self, category, id, name);
  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(GfxModel), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    GfxModelCtor(mem, name, 0);
    model = mem;
  }
  self->models[cell] = model;

  /* rootMatrix = rotation about Y by 3.14 (vmul.s by S703 then vrot in quarter turns) */
  m = model->data->rootMatrix;
  c = __builtin_cosf(3.14f);
  sn = __builtin_sinf(3.14f);
  m[0] = c;
  m[1] = 0.0f;
  m[2] = -sn;
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = 1.0f;
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = sn;
  m[9] = 0.0f;
  m[10] = c;
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;

  /* each field transformed by a rotation about X by the cell's tilt (vmmul; the matrix pointer is
     read before the call) */
  m = self->models[cell]->data->rootMatrix;
  tilt = UiCollectionSphereGetModelTilt(self, cell);
  c = __builtin_cosf(tilt);
  sn = __builtin_sinf(tilt);
  for (i = 0; i < 4; i++) {
    float r[4];

    r[0] = m[i * 4 + 0];
    r[1] = m[i * 4 + 1];
    r[2] = m[i * 4 + 2];
    r[3] = m[i * 4 + 3];
    UiCollectionSphereLoadRotXField(&m[i * 4], r, c, sn);
  }

  self->modelScale[cell] = 0.4f;
  m = self->models[cell]->data->rootMatrix;
  for (i = 0; i < 3; i++) {
    m[i * 4 + 0] = m[i * 4 + 0] * 0.4f;
    m[i * 4 + 1] = m[i * 4 + 1] * 0.4f;
    m[i * 4 + 2] = m[i * 4 + 2] * 0.4f;
    m[i * 4 + 3] = m[i * 4 + 3] * 0.4f;
  }

  self->models[cell]->pos[0] = 0.0f;
  self->models[cell]->pos[1] = 0.0f;
  self->models[cell]->pos[2] = 0.0f;
  model = self->models[cell];
  entry = &((const VtblEntry *)model->base.vtable)[6];
  ((float (*)(void *, float))entry->fn)((u8 *)model + entry->delta, 0.0f);
  self->models[cell]->lighting = 1;
  specular[0] = 0.4f;
  specular[1] = 0.4f;
  specular[2] = 0.4f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(8.0f, self->models[cell], specular, NULL);

  /* translation row = pos (all four words) */
  model = self->models[cell];
  m = model->data->rootMatrix;
  m[12] = model->pos[0];
  m[13] = model->pos[1];
  m[14] = model->pos[2];
  m[15] = model->pos[3];
  self->models[cell]->data->rootMatrix[15] = 1.0f;
  self->models[cell]->ambient[3] = 0.0f;
  GfxModelForEachMaterial(self->models[cell], (void *)UiCollectionSphereModelMaterialCallback, NULL);
}
