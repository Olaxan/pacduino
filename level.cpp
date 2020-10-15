#include <SPI.h>
#include <GD2.h>

#include "level.h"

#define NONE_INDEX -1
#define POWER_INDEX 27
#define PELLET_INDEX 29

level::level(int width, int height, int sprite_handle)
		: width_(width), height_(height), sprite_handle_(sprite_handle) 
{

	data_ = new int[width_ * height_] 
	{ 
		3, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 5, 
		16, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 16, 27, 16, 
		16, 29, 0, 1, 1, 2, 29, 0, 1, 1, 1, 2, 29, 16, 29, 16, 
		16, 29, 10, 11, 11, 12, 29, 10, 11, 11, 11, 12, 29, 16, 29, 16, 
		16, 29, 20, 21, 21, 22, 29, 20, 21, 21, 21, 22, 29, 26, 29, 16, 
		16, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 16, 
		16, 29, 0, 1, 1, 2, 29, 3, 5, 29, 3, 8, 8, 8, 8, 15, 
		16, 29, 20, 21, 21, 22, 29, 16, 16, 29, 23, 8, 8, 4, 8, 15, 
		26, 29, 29, 29, 29, 29, 29, 16, 16, 29, 29, 29, 29, 16, 27, 16, 
		8, 8, 8, 5, 29, 6, 29, 16, 23, 8, 8, 5, 29, 16, 29, 16, 
		-1, -1, -1, 16, 29, 16, 29, 16, 3, 8, 8, 25, 29, 26, 29, 16, 
		-1, -1, -1, -1, 29, 16, 29, 16, 16, 29, 29, 29, 29, 29, 29, 16, 
		-1, -1, -1, 16, 29, 16, 29, 16, 16, 29, 0, 1, 1, 2, 29, 16, 
		8, 8, 8, 25, 29, 26, 29, 23, 25, 29, 20, 21, 21, 22, 29, 16, 
		6, 27, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 16, 
		23, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 25, 
	};
}

float level::screen_scale(int screen_width, int screen_height, int cell_size)
{
	float tile_x = float(screen_width) / float(cell_size * width_);
	float tile_y = float(screen_height) / float(cell_size * height_);
 	scale_ = min(tile_x, tile_y);
	tile_size_ = scale_ * cell_size;

	return scale_;
}

int level::fit_cell(int xy)
{
	return xy * tile_size_;
}

bool level::is_free(int x, int y)
{
	int tile = data_[y * width_ + x];
	return (tile == NONE_INDEX || tile == PELLET_INDEX || tile == POWER_INDEX); 
}

bool level::is_free(int x, int y, direction dir, int step)
{
	switch (dir)
	{
		case direction::up:
			return is_free(x, y - step);

		case direction::down:
			return is_free(x, y + step);

		case direction::left:
			return is_free(x - step, y);

		case direction::right:
			return is_free(x + step, y);
		
		default:
			return is_free(x, y);
	}
}

bool level::is_intersection(int x, int y)
{
	int open = 0;
	for (size_t i = 0; i < 4; i++)
		open += is_free(x, y, static_cast<direction>(i));

	return open > 2;
}

bool level::is_corridor(int x, int y)
{
	if (is_intersection(x, y))
		return false;

	bool a = is_free(x, y, direction::up) && is_free(x, y, direction::down);
	bool b = is_free(x, y, direction::left) && is_free(x, y, direction::right);

	return a ^ b;
}

void level::pickup(int x, int y)
{
	int& tile_item = data_[y * width_ + x];

	switch (tile_item)
	{
		case PELLET_INDEX:
			if (on_pickup_pellet != nullptr)	
				(*on_pickup_pellet)();
			break;

		case POWER_INDEX:
			if (on_pickup_power != nullptr)
				(*on_pickup_power)();
			break;	
	}	

	tile_item = NONE_INDEX;
}

void level::draw()
{	
	size_t size = width_ * height_;

	for (size_t i = 0; i < size; i++)
	{
		if (data_[i] == -1)
			continue;

		GD.Vertex2ii(tile_size_ * (i % width_), tile_size_ * (i / width_), sprite_handle_, data_[i]);
	}
}
