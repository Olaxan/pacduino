#include "pac.h"

#include <SPI.h>
#include <GD2.h>

#define PI 3.14159265359f

player::player(level* level, int x, int y, int cell, int cell_offset, int cell_count) :
	   	level_(level), x_(x), y_(y), prev_x_(x), prev_y_(y), 
		next_dir_(direction::right), dir_(direction::right), 
		cell_(cell), cell_offset_(cell_offset),  cell_count_(cell_count) 
{}

void player::set_direction(direction dir)
{
	next_dir_ = dir;
}

void player::set_direction(int x, int y)
{
	if (x != 0)
	{
		next_dir_ = (x > 0) ? direction::right : direction::left;
		return;
	}

	if (y != 0)
	{
		next_dir_ = (y > 0) ? direction::down : direction::up;
		return;
	}
}

void player::move()
{
	prev_x_ = x_;
	prev_y_ = y_;

	if (level_->is_free(x_, y_, next_dir_))
		dir_ = next_dir_;

	if (level_->is_free(x_, y_, dir_))
	{
		y_ += (dir_ == direction::down) - (dir_ == direction::up);
		x_ += (dir_ == direction::right) - (dir_ == direction::left);
	}
}

void player::step()
{
	move();
	level_->pickup(prev_x_, prev_y_);	
}

void player::draw(float alpha)
{
	int x1 = level_->fit_cell(prev_x_);
	int y1 = level_->fit_cell(prev_y_);
	int x2 = level_->fit_cell(x_);
	int y2 = level_->fit_cell(y_);
	int x = x1 + alpha * (x2 - x1);
	int y = y1 + alpha * (y2 - y1);

	int s = get_sprite(alpha);

	GD.Vertex2ii(x, y, cell_, s);	
}

int player::get_sprite(float alpha)
{
	return cell_offset_ + static_cast<int>(dir_) * cell_count_ + round(sin(alpha * PI) * float(cell_count_ - 1)); 
}
