#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>
#include <SD.h>

#include "assets.h"
#include "level.h"
#include "pac.h"
#include "ghosts.h"
#include "config.h"

#define error(msg) {Serial.println(F(msg)); while(1);}

const int joy_x = A8;
const int joy_y = A9;
const int joy_b = 22;

const int axis[] = {joy_x, joy_y};  	// pin numbers for {x, y}
const int range = 12;					// output range of X or Y movement
const int responseDelay = 500;			// response delay of the mouse, in ms
const int threshold = range / 4;		// resting threshold
const int center = range / 2;			// resting position value

int minima[] = {0, 0};			// actual analogRead minima for {x, y}
int maxima[] = {1023, 1023};	// actual analogRead maxima for {x, y}

float scale = 1.0f;

int frame = 0;
int score = 0;
int kills = 0;
int pellets = 0;

level lvl = level(LEVEL_WIDTH, LEVEL_HEIGHT, VTILES_HANDLE);
player pac = player(&lvl, 1, 1, SPRITES2_HANDLE, 0, 3);

ghost ghosts[2] = {
	ghost(&lvl, &pac, 0, 10, SPRITES2_HANDLE, 12, 2, 44, 2), 
	ghost(&lvl, &pac, 0, 12, SPRITES2_HANDLE, 20, 2, 44, 2) 
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

	GD.ClearColorRGB(0);

	GD.BitmapHandle(0);
	GD.BitmapLayout(ARGB2, 32, 32);
	GD.BitmapHandle(1);
	GD.BitmapLayout(ARGB2, 16, 16);
	GD.BitmapHandle(2);
	GD.BitmapLayout(ARGB2, 480, 347);
		
	Serial.println("Game ready.");

	scale = lvl.screen_scale(SCREEN_WIDTH, SCREEN_HEIGHT, VTILES_WIDTH);
	
	// Setup events
	lvl.on_pickup_pellet = &on_pickup_pellet;
	lvl.on_pickup_power = &on_pickup_power;

	for (auto& g : ghosts)
	{
		g.on_death_ghost = &on_death_ghost;
		g.on_death_player = &on_death_player;
	}

	pellets = lvl.count(PELLET_INDEX);

	splash();
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
	int frame_offset = LEVEL_WIDTH * VTILES_WIDTH * scale + 10; 
	GD.Clear();

	GD.cmd_text(frame_offset - 5, 5, CRACKMAN_HANDLE, 0, "KNARKMAN");	
	GD.cmd_text(frame_offset, 40, 21, 0, String("Pellets: " + String(score) + " / " + String(pellets)).c_str());
	GD.cmd_text(frame_offset, 55, 21, 0, String("Kills: " + String(kills)).c_str());

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

void splash()
{
	GD.Clear();
	GD.cmd_text(5, 5, CRACKMAN_HANDLE, 0, "KNARKMAN");
	GD.cmd_text(10, 40, 21, 0, "Tonto Turbo Inc. 2020");
	GD.Begin(BITMAPS);
	GD.Vertex2ii(100, 0, SPLASH_HANDLE);
	GD.swap();
	delay(SPLASH_DELAY);
}

void win()
{
	GD.Clear();
	GD.cmd_text(240, 106, CRACKMAN_HANDLE, OPT_CENTER, "Bra knarkat");
	GD.cmd_text(240, 141, 21, OPT_CENTER, "You got all the pellets!"); 
	GD.swap();
	delay(SPLASH_DELAY);
	reset();
}

void on_pickup_pellet()
{
	GD.sample(WAKA, WAKA_LENGTH, WAKA_FREQ, ADPCM_SAMPLES);

	score++;
	if (score == pellets)
		win();
}

void on_pickup_power()
{
	GD.sample(POWER, POWER_LENGTH, POWER_FREQ, ADPCM_SAMPLES);

	for (auto& g : ghosts)
		g.set_scared(true);
}

void on_death_player()
{
	GD.sample(DEATH, DEATH_LENGTH, DEATH_FREQ, ADPCM_SAMPLES);
	reset();
}

void on_death_ghost()
{
	GD.sample(MURDER, MURDER_LENGTH, MURDER_FREQ, ADPCM_SAMPLES);
	kills++;
}
