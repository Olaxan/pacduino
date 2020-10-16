// This file was generated with the command-line:
//    /usr/bin/gd2asset -f assets2.gd2 -o assets.h vtiles.png,format=ARGB2 sprites2.png,format=ARGB2 splash.png,format=ARGB2 ttf/crackman.ttf,size=32 waka.wav

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
#define SPLASH_HEIGHT 272
#define SPLASH_CELLS 1
#define CRACKMAN_HANDLE 3
#define CRACKMAN_WIDTH 34
#define CRACKMAN_HEIGHT 34
#define CRACKMAN_CELLS 96
#define WAKA 229208UL
#define WAKA_LENGTH 7936
#define WAKA_FREQ 16000
#define ASSETS_END 237144UL
#define LOAD_ASSETS()  (GD.safeload("assets2.gd2"), GD.loadptr = ASSETS_END)
static const shape_t VTILES_SHAPE = {0, 32, 960, 0};
static const shape_t SPRITES2_SHAPE = {1, 16, 768, 0};
static const shape_t SPLASH_SHAPE = {2, 480, 272, 0};
static const shape_t CRACKMAN_SHAPE = {3, 34, 34, 0};
struct {
  Bitmap vtiles;
  Bitmap sprites2;
  Bitmap splash;
  Bitmap crackman;
} bitmaps = {
 /*           vtiles */  {{ 32, 960}, { 16, 480},      0x0UL,  5,  0},
 /*         sprites2 */  {{ 16, 768}, {  8, 384},   0x7800UL,  5,  1},
 /*           splash */  {{480, 272}, {240, 136},   0xa800UL,  5,  2},
 /*         crackman */  {{ 34,  34}, { 17,  17},  0x2a600UL,  2,  3}
};
