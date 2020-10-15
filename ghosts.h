#include "pac.h"

class ghost : public player
{
private:
	player* player_;
	int home_x_, home_y_, scared_offset_, scared_count_, scared_steps_;
	bool is_scared_;
public:

	ghost(level* level, player* player, int x, int y, int cell, int cell_offset, int cell_count, int scared_offset, int scared_count);

	void (*on_death_player)(void);
	void (*on_death_ghost)(void);

	void set_scared(bool is_scared);
	void step() override;

	int get_sprite(float alpha) override;
};
