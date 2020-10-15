#include "ghosts.h"

#include <SPI.h>
#include <GD2.h>

#define MAX_SCARED_STEPS 30
#define SCARED_STEPS_BLINK 10

ghost::ghost(level* level, player* pacman, int x, int y, int cell, int cell_offset, int cell_count, int scared_offset, int scared_count) : 
	player{ level, x, y, cell, cell_offset, cell_count }, player_(pacman), home_x_(x), home_y_(y),	
	scared_offset_(scared_offset), scared_count_(scared_count), scared_steps_(0), is_scared_(false)	
{}

void ghost::step()
{
	if (!level_->is_corridor(x_, y_))
		while (!level_->is_free(x_, y_, next_dir_ = static_cast<direction>(GD.random(4))));

	move();	

	if (player_->is_at(x_, y_))
	{
		if (is_scared_)
		{
			on_death_ghost();
			x_ = home_x_;
			y_ = home_y_;
			is_scared_ = false;
		}
		else
			on_death_player();
	}

	scared_steps_--;
	
	if (scared_steps_ < 0)
		is_scared_ = false;
}

void ghost::set_scared(bool is_scared)
{
	is_scared_ = true;
	scared_steps_ = MAX_SCARED_STEPS;
}

int ghost::get_sprite(float alpha)
{
	if (is_scared_)
	{
		int blink_offset = scared_count_ * (scared_steps_ < SCARED_STEPS_BLINK && scared_steps_ % 2 == 0);
		return scared_offset_ + blink_offset + round(sin(alpha * PI) * float(scared_count_ - 1)); 
	}
	else
		return player::get_sprite(alpha);
			
}
