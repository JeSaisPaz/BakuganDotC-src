// bdc 0x0898c278 UiCollectionFigureLoadCellModel
#include "bdc.h"

/* Loads the 3D figure model of grid cell `cell` of the figure collection screen (task 314,
   `maybe_UiScreen314Ctor`): builds the GMO file name of entry `id` of `category`
   (`UiCollectionFigureBuildModelName`, e.g. `"01_V_Ingram_N_U_figure.gmo"`), allocates a
   0x140-byte model from the low end of the heap and constructs it (`GfxModelCtor`), stores it in
   `models[cell]` (NULL when the allocation failed; the following accesses go through it), then sets
   its GMO root matrix to rotY(3.14), then to the VFPU product `vmmul.q E200, E100, E000` of it with
   rotX(-0) (field j lane i = sum_k m[j][k] * rot[k][i], i.e. M200 = rot * m),
   scales the first three rows by `baseScale` (also copied to `cellScale[cell]`), clears `pos`, sets
   the motion speed to 0 (vtable slot 6, `GfxModelSetMotionSpeed`), turns lighting on, sets
   specular `{0.4, 0.4, 0.4, 1}` power 8 (`GfxModelSetSpecular`), copies `pos` into the matrix
   translation row with w = 1, clears `ambient[3]` and sets the stencil reference to `cell`
   (`GfxModelSetStencilRef`). `unused` is not read. */
void UiCollectionFigureLoadCellModel(UiCollectionFigure *self, u8 category, u8 id, u32 unused, u8 cell)
{
  float specular[4];
  char name[72];
  GfxModel *model;
  GfxModel *mem;
  float *m;
  const VtblEntry *entry;
  bool fromLow;
  float s;
  float c;
  float sn;
  float rot[16];
  float tmp[16];
  int i;
  int j;

  UiCollectionFigureBuildModelName(self, category, id, name);
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

  /* X rotation by -0: fields (1,0,0,0), (0,c,s,0), (0,-s,c,0), (0,0,0,1). */
  m = self->models[cell]->data->rootMatrix;
  c = __builtin_cosf(-0.0f);
  sn = __builtin_sinf(-0.0f);
  rot[0] = 1.0f;
  rot[1] = 0.0f;
  rot[2] = 0.0f;
  rot[3] = 0.0f;
  rot[4] = 0.0f;
  rot[5] = c;
  rot[6] = sn;
  rot[7] = 0.0f;
  rot[8] = 0.0f;
  rot[9] = -sn;
  rot[10] = c;
  rot[11] = 0.0f;
  rot[12] = 0.0f;
  rot[13] = 0.0f;
  rot[14] = 0.0f;
  rot[15] = 1.0f;
  /* vmmul.q E200, E100 (m), E000 (rot). All-E form: E_d = E_s * E_t as named, so
     M200 = M000 * M100 = rot * m (fields as columns), the rule lift-diff-proven by GmoMat4Mul and
     vmmul_q_transp3 (`vmmul.q E000, E200, E100` = M100 * M200; the A*B^T and B*A forms failed):
     field j lane i = sum_k m[j][k] * rot[k][i]. With rot = rotX(-0) = identity the result is m
     itself, so rotY(3.14) is kept untransposed. */
  for (j = 0; j < 4; j++) {
    for (i = 0; i < 4; i++) {
      tmp[j * 4 + i] = m[j * 4 + 0] * rot[0 * 4 + i] + m[j * 4 + 1] * rot[1 * 4 + i] +
                       m[j * 4 + 2] * rot[2 * 4 + i] + m[j * 4 + 3] * rot[3 * 4 + i];
    }
  }
  for (i = 0; i < 16; i++) {
    m[i] = tmp[i];
  }

  s = self->baseScale;
  self->cellScale[cell] = s;
  m = self->models[cell]->data->rootMatrix;
  for (i = 0; i < 3; i++) {
    m[i * 4 + 0] = m[i * 4 + 0] * s;
    m[i * 4 + 1] = m[i * 4 + 1] * s;
    m[i * 4 + 2] = m[i * 4 + 2] * s;
    m[i * 4 + 3] = m[i * 4 + 3] * s;
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
  GfxModelSetStencilRef(self->models[cell], cell);
}
