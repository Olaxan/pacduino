// This file was generated with the command-line:
//    /usr/bin/gd2asset -f assets.gd2 -o assets.h vtiles.png,format=ARGB2 sprites2.png,format=ARGB2 splash.png,format=ARGB2

#define VTILES_HANDLE 0
#define VTILES_WIDTH 32
#define VTILES_HEIGHT 960
#define VTILES_CELLS 1
#define SPRITES2_HANDLE 1
#define SPRITES2_WIDTH 16
#define SPRITES2_HEIGHT 768
#define SPRITES2_CELLS 1
#define SPLASH_HANDLE 2
#define SPLASH_WIDTH 480
#define SPLASH_HEIGHT 347
#define SPLASH_CELLS 1
#define ASSETS_END 209568UL
#define LOAD_ASSETS()  (GD.safeload("assets.gd2"), GD.loadptr = ASSETS_END)
static const shape_t VTILES_SHAPE = {0, 32, 960, 0};
static const shape_t SPRITES2_SHAPE = {1, 16, 768, 0};
static const shape_t SPLASH_SHAPE = {2, 480, 347, 0};
struct {
  Bitmap vtiles;
  Bitmap sprites2;
  Bitmap splash;
} bitmaps = {
 /*           vtiles */  {{ 32, 960}, { 16, 480},      0x0UL,  5,  0},
 /*         sprites2 */  {{ 16, 768}, {  8, 384},   0x7800UL,  5,  1},
 /*           splash */  {{480, 347}, {240, 173},   0xa800UL,  5,  2}
};
