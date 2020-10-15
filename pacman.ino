#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>
#include <SD.h>

#include "assets.h"
#include "level.h"
#include "pac.h"
#include "ghosts.h"

#define LEVEL_PATH "level1.csv"
#define LEVEL_WIDTH 16
#define LEVEL_HEIGHT 16

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

#define TICK_RATE 20
#define READ_RATE 10

#define error(msg) {Serial.println(F(msg)); while(1);}

const int joy_x = A8;
const int joy_y = A9;
const int joy_b = 22;

const int axis[] = {joy_x, joy_y};  	// pin numbers for {x, y}
const int range = 12;			// output range of X or Y movement
const int responseDelay = 500;		// response delay of the mouse, in ms
const int threshold = range / 4;	// resting threshold
const int center = range / 2;		// resting position value

int minima[] = {1023, 1023};		// actual analogRead minima for {x, y}
int maxima[] = {0, 0};			// actual analogRead maxima for {x, y}

float scale = 1.0f;

int frame = 0;
int score = 0;
int kills = 0;

level lvl = level(LEVEL_WIDTH, LEVEL_HEIGHT, VTILES_HANDLE);
player pac = player(&lvl, 1, 1, SPRITES_HANDLE, 0, 3);

ghost ghosts[2] = {
	ghost(&lvl, &pac, 0, 10, SPRITES_HANDLE, 12, 2, 44, 2), 
	ghost(&lvl, &pac, 0, 12, SPRITES_HANDLE, 20, 2, 44, 2) 
};

void (*reset) (void) = 0;

void setup() 
{
	Serial.begin(9600);
	while (!Serial);
	
	Serial.println("Init SPI...");
	
	//read_level(LEVEL_PATH, LEVEL_WIDTH, LEVEL_HEIGHT, level);	

	Serial.println("Init Gameduino2...");
	GD.begin();

	Serial.println("Loading GFX...");
	LOAD_ASSETS();

	GD.BitmapHandle(0);
	GD.BitmapLayout(ARGB4, 2 * 32, 32);
	GD.BitmapHandle(1);
	GD.BitmapLayout(ARGB4, 2 * 16, 16);
		
	Serial.println("Game ready.");

	scale = lvl.screen_scale(SCREEN_WIDTH, SCREEN_HEIGHT, VTILES_WIDTH);

	lvl.on_pickup_pellet = &on_pickup_pellet;
	lvl.on_pickup_power = &on_pickup_power;

	for (auto& g : ghosts)
	{
		g.on_death_ghost = &on_death_ghost;
		g.on_death_player = &on_death_player;
	}
}

void loop() 
{

	if (frame % READ_RATE == 0)
	{
		// Do not read input every frame, since it leads to performance loss.
		int dx = sign(read_axis(0));
		int dy = sign(read_axis(1));

		pac.set_direction(dx, dy);
	}

	if (frame % TICK_RATE == 0)
	{
		pac.step();
		
		for (auto& g : ghosts)
			g.step();

		frame = 0;
	}

	/* ---------- RENDERING ---------- */

	float frame_alpha = float(frame) / float(TICK_RATE);

	GD.ClearColorRGB(0);
	GD.Clear();

	GD.cmd_text(LEVEL_WIDTH * VTILES_WIDTH * scale + 10, 10, 21, 0, String("Pellets: " + String(score)).c_str());
	GD.cmd_text(LEVEL_WIDTH * VTILES_WIDTH * scale + 10, 25, 21, 0, String("Kills: " + String(kills)).c_str());
	
	// Apply level scaling
	GD.Begin(BITMAPS);
	GD.cmd_scale(F16(scale), F16(scale));
	GD.cmd_setmatrix();
	lvl.draw();

	GD.cmd_loadidentity();
	
	// Apply sprite scaling
	GD.cmd_scale(F16(2 * scale), F16(2 * scale));
	GD.cmd_setmatrix();

	pac.draw(frame_alpha);

	for (auto& g : ghosts)
		g.draw(frame_alpha);

	GD.swap();
	
	frame++;
}

void on_pickup_pellet()
{
	score++;
}

void on_pickup_power()
{
	for (auto& g : ghosts)
		g.set_scared(true);
}

void on_death_player()
{
	reset();
}

void on_death_ghost()
{
	kills++;
}

int wrap(int val, int const lower, int const upper)
{
	int range_size = upper - lower + 1;

	if (val < lower)
		val += range_size * ((lower - val) / range_size + 1);

	return lower + (val - lower) % range_size;
}

int read_axis(int axisNumber) 
{
	int distance = 0; 

	int reading = analogRead(axis[axisNumber]);

	if (reading < minima[axisNumber]) {
		minima[axisNumber] = reading;
	}
	if (reading > maxima[axisNumber]) {
		maxima[axisNumber] = reading;
	}

	reading = map(reading, minima[axisNumber], maxima[axisNumber], 0, range);

	if (abs(reading - center) > threshold) {
		distance = (reading - center);
	}

	if (axisNumber == 1) {
		distance = -distance;
	}

	return distance;
}

int sign(int val) 
{
	if (val < 0) return -1;
	if (val == 0) return 0;
	return 1;
}
