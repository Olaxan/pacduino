#ifndef PAC_H_
#define PAC_H_

#include "level.h"


class player
{

protected:

	level* level_;
	int x_, y_, prev_x_, prev_y_, cell_, cell_offset_, cell_count_;
	direction next_dir_, dir_;

public:

	player(level* level, int x, int y, int cell, int cell_offset, int cell_count);

	void (*on_step) (void);

	void set_direction(direction dir);
	void set_direction(int x, int y);
	
	int x() const { return x_; }
	int y() const { return y_; }

	bool was_at(int x, int y) { return x == prev_x_ && y == prev_y_; }
	bool is_at(int x, int y) { return x == x_ && y == y_; }

	virtual void move();
	virtual void step();
	virtual void draw(float alpha);

	virtual int get_sprite(float alpha);
};

#endif
