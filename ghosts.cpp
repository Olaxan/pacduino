#include "ghosts.h"
#include "config.h"

#include <SPI.h>
#include <GD2.h>

ghost::ghost(level* level, player* pacman, int x, int y, int cell, int cell_offset, int cell_count, int scared_offset, int scared_count) : 
	player{ level, x, y, cell, cell_offset, cell_count }, player_(pacman), home_x_(x), home_y_(y),	
	scared_offset_(scared_offset), scared_count_(scared_count), scared_steps_(0), is_scared_(false)	
{}

void ghost::step()
{
	if (!level_->is_corridor(x_, y_))
		while (!level_->is_free(x_, y_, next_dir_ = static_cast<direction>(GD.random(4))));

	move();	

	if (player_->is_at(x_, y_) || player_->is_at(prev_x_, prev_y_))
	{
		if (is_scared_)
		{
			if (on_death_ghost != nullptr) 
				on_death_ghost();

			x_ = home_x_;
			y_ = home_y_;
			is_scared_ = false;
		}
		else if (on_death_player != nullptr)
			on_death_player();
	}
	
	if (scared_steps_ < 0)
		is_scared_ = false;
	else
		scared_steps_--;
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
