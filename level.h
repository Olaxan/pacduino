#ifndef LEVEL_H_
#define LEVEL_H_

enum class direction
{
	up,
	right,
	down,
	left
};

enum class item
{
	none = -1,
	pellet = 29,
	power_pellet = 28
};

class level
{
private:
	int width_, height_, tile_size_, sprite_handle_;
	float scale_;

	int* data_;

public:

	level(int width, int height, int sprite_handle);

	float screen_scale(int screen_width, int screen_height, int cell_size); 
	int fit_cell(int xy);
	
	void (*on_pickup_pellet)(void);
	void (*on_pickup_power)(void);

	bool is_free(int x, int y);
	bool is_free(int x, int y, direction dir, int step = 1);
	bool is_intersection(int x, int y);
	bool is_corridor(int x, int y);

	void pickup(int x, int y);
	void draw();
};

#endif
