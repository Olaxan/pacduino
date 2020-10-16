#ifndef CONFIG_H_
#define CONFIG_H_

// Constants
#define PI 3.14159265359f

// Screen / Game
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272
#define SPLASH_DELAY 2000

#define TICK_RATE 20
#define READ_RATE 10

// Level data (should be dynamic really)
#define LEVEL_PATH "level1.csv"
#define LEVEL_WIDTH 16
#define LEVEL_HEIGHT 16

// Item indices
#define NONE_INDEX -1
#define POWER_INDEX 27
#define PELLET_INDEX 29

// Ghost behaviour
#define MAX_SCARED_STEPS 30
#define SCARED_STEPS_BLINK 10

#endif
