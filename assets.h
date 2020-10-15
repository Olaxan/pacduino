// This file was generated with the command-line:
//    /usr/bin/gd2asset -o ../assets.h -f assets.gd2 vtiles.png sprites.png

#define VTILES_HANDLE 0
#define VTILES_WIDTH 32
#define VTILES_HEIGHT 960
#define VTILES_CELLS 1
#define SPRITES_HANDLE 1
#define SPRITES_WIDTH 16
#define SPRITES_HEIGHT 768
#define SPRITES_CELLS 1
#define ASSETS_END 86016UL
#define LOAD_ASSETS()  (GD.safeload("assets.gd2"), GD.loadptr = ASSETS_END)
static const shape_t VTILES_SHAPE = {0, 32, 960, 0};
static const shape_t SPRITES_SHAPE = {1, 16, 768, 0};
struct {
  Bitmap vtiles;
  Bitmap sprites;
} bitmaps = {
 /*           vtiles */  {{ 32, 960}, { 16, 480},      0x0UL,  6,  0},
 /*          sprites */  {{ 16, 768}, {  8, 384},   0xf000UL,  6,  1}
};
