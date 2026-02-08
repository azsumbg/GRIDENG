#include "pch.h"
#include "grideng.h"

// RANDIT CLASS ************************

dll::RANDIT::RANDIT()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd(), rd() };

	twister = new std::mt19937(sq);
}
dll::RANDIT::~RANDIT()
{
	if (twister)delete twister;
}

int dll::RANDIT::operator ()(int min, int max)
{
	std::uniform_int_distribution gen(min, max);

	return gen(*twister);
}
float dll::RANDIT::operator ()(float min, float max)
{
	std::uniform_real_distribution<float> gen(min, max);

	return gen(*twister);
}

////////////////////////////////////////

// PROTON CLASS ************************

dll::PROTON::PROTON()
{
	_width = 1.0f;
	_height = 1.0f;

	start.x = 1.0f;
	start.y = 1.0f;

	x_radius = _width / 2.0f;
	y_radius = _height / 2.0f;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;

	end.x = start.x + _width;
	end.y = start.y + _height;
}
dll::PROTON::PROTON(float firstx, float firsty) :start{ firstx, firsty }
{
	_width = 1.0f;
	_height = 1.0f;

	x_radius = _width / 2.0f;
	y_radius = _height / 2.0f;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;

	end.x = start.x + _width;
	end.y = start.y + _height;
}
dll::PROTON::PROTON(float firstx, float firsty, float firstw, float firsth) :start{ firstx,firsty },
_width{ firstw }, _height{ firsth }
{
	x_radius = _width / 2.0f;
	y_radius = _height / 2.0f;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;

	end.x = start.x + _width;
	end.y = start.y + _height;
}

void dll::PROTON::set_edges()
{
	center.x = start.x + x_radius;
	center.y = start.y + y_radius;

	end.x = start.x + _width;
	end.y = start.y + _height;
}
void dll::PROTON::new_dims(float new_width, float new_height)
{
	_width = new_width;
	_height = new_height;

	x_radius = _width / 2.0f;
	y_radius = _height / 2.0f;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;

	end.x = start.x + _width;
	end.y = start.y + _height;
}
void dll::PROTON::set_width(float new_width)
{
	_width = new_width;

	x_radius = _width / 2.0f;

	center.x = start.x + x_radius;

	end.x = start.x + _width;
}
void dll::PROTON::set_height(float new_height)
{
	_height = new_height;

	y_radius = _height / 2.0f;

	center.y = start.y + y_radius;

	end.y = start.y + _height;
}

float dll::PROTON::get_width()const
{
	return _width;
}
float dll::PROTON::get_height()const
{
	return _height;
}

////////////////////////////////////////

// CLASS GRID **************************

dll::GRID::GRID()
{
	float my_x = left_tile_x;
	float my_y = up_tile_y;

	for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
	{
		for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
		{
			int atype = RandIt(0, 4);

			grid[rows][cols].type = static_cast<tiles>(atype);

			grid[rows][cols].dims.up = my_y;
			grid[rows][cols].dims.left = my_x;
			grid[rows][cols].dims.right = grid[rows][cols].dims.left + 49.0f;
			grid[rows][cols].dims.down = grid[rows][cols].dims.up + 49.0f;

			switch (grid[rows][cols].type)
			{
			case tiles::dirt:
				grid[rows][cols].move_reduct = 0;
				break;

			case tiles::forest:
				grid[rows][cols].move_reduct = -0.5f;
				break;

			case tiles::lava:
				grid[rows][cols].move_reduct = -3.5f;
				break;

			case tiles::path:
				grid[rows][cols].move_reduct = +0.5f;
				break;

			case tiles::water:
				grid[rows][cols].move_reduct = -8.0f;
				break;
			}

			my_x += 50.0f;
		}

		my_y += 50.0f;
	}
	
}

void dll::GRID::add_row(dirs where)
{
	if (where == dirs::up)
	{
		for (int rows = 0; rows < GRID_MAX_ROWS - 1; ++rows)
		{
			for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
			{
				grid[rows + 1][cols].type = grid[rows][cols].type;
				grid[rows + 1][cols].dims = grid[rows][cols].dims;
				grid[rows + 1][cols].move_reduct = grid[rows][cols].move_reduct;
			}
		}

		float my_x = left_tile_x;

		for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
		{
			int atype = RandIt(0, 4);

			grid[0][cols].type = static_cast<tiles>(atype);

			grid[0][cols].dims.up = up_tile_y;
			grid[0][cols].dims.left = my_x;
			grid[0][cols].dims.right = grid[0][cols].dims.left + 49.0f;
			grid[0][cols].dims.down = grid[0][cols].dims.up + 49.0f;

			switch (grid[0][cols].type)
			{
			case tiles::dirt:
				grid[0][cols].move_reduct = 0;
				break;

			case tiles::forest:
				grid[0][cols].move_reduct = -0.5f;
				break;

			case tiles::lava:
				grid[0][cols].move_reduct = -3.5f;
				break;

			case tiles::path:
				grid[0][cols].move_reduct = +0.5f;
				break;

			case tiles::water:
				grid[0][cols].move_reduct = -8.0f;
				break;
			}

			my_x += 50.0f;
		}
	}
	else if (where == dirs::down)
	{
		for (int rows = GRID_MAX_ROWS - 1; rows > 0 ; --rows)
		{
			for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
			{
				grid[rows - 1][cols].type = grid[rows][cols].type;
				grid[rows - 1][cols].dims = grid[rows][cols].dims;
				grid[rows - 1][cols].move_reduct = grid[rows][cols].move_reduct;
			}
		}

		float my_x = left_tile_x;

		for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
		{
			int atype = RandIt(0, 4);

			grid[GRID_MAX_ROWS - 1][cols].type = static_cast<tiles>(atype);

			grid[GRID_MAX_ROWS - 1][cols].dims.up = up_tile_y;
			grid[GRID_MAX_ROWS - 1][cols].dims.left = my_x;
			grid[GRID_MAX_ROWS - 1][cols].dims.right = grid[0][cols].dims.left + 49.0f;
			grid[GRID_MAX_ROWS - 1][cols].dims.down = grid[0][cols].dims.up + 49.0f;

			switch (grid[GRID_MAX_ROWS - 1][cols].type)
			{
			case tiles::dirt:
				grid[GRID_MAX_ROWS - 1][cols].move_reduct = 0;
				break;

			case tiles::forest:
				grid[GRID_MAX_ROWS - 1][cols].move_reduct = -0.5f;
				break;

			case tiles::lava:
				grid[GRID_MAX_ROWS - 1][cols].move_reduct = -3.5f;
				break;

			case tiles::path:
				grid[GRID_MAX_ROWS - 1][cols].move_reduct = +0.5f;
				break;

			case tiles::water:
				grid[GRID_MAX_ROWS - 1][cols].move_reduct = -8.0f;
				break;
			}

			my_x += 50.0f;
		}
	}
}

void dll::GRID::add_col(dirs where)
{
	if (where == dirs::left)
	{
		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			for (int cols = 0; cols < GRID_MAX_COLS - 1; ++cols)
			{ 
				grid[rows][cols + 1].type = grid[rows][cols].type;
				grid[rows][cols + 1].dims = grid[rows][cols].dims;
				grid[rows][cols + 1].move_reduct = grid[rows][cols].move_reduct;
			}
		}

		float my_y = up_tile_y;

		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			int atype = RandIt(0, 4);

			grid[rows][0].type = static_cast<tiles>(atype);

			grid[rows][0].dims.up = my_y;
			grid[rows][0].dims.left = left_tile_x;
			grid[rows][0].dims.right = grid[rows][0].dims.left + 49.0f;
			grid[rows][0].dims.down = grid[rows][0].dims.up + 49.0f;

			switch (grid[rows][0].type)
			{
			case tiles::dirt:
				grid[rows][0].move_reduct = 0;
				break;

			case tiles::forest:
				grid[rows][0].move_reduct = -0.5f;
				break;

			case tiles::lava:
				grid[rows][0].move_reduct = -3.5f;
				break;

			case tiles::path:
				grid[rows][0].move_reduct = +0.5f;
				break;

			case tiles::water:
				grid[rows][0].move_reduct = -8.0f;
				break;
			}

			my_y += 50.0f;
		}
	}
	else if (where == dirs::right)
	{
		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			for (int cols = GRID_MAX_COLS - 1; cols > 0; --cols)
			{
				grid[rows][cols - 1].type = grid[rows][cols].type;
				grid[rows][cols - 1].dims = grid[rows][cols].dims;
				grid[rows][cols - 1].move_reduct = grid[rows][cols].move_reduct;
			}
		}

		float my_y = up_tile_y;

		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			int atype = RandIt(0, 4);

			grid[rows][GRID_MAX_COLS - 1].type = static_cast<tiles>(atype);

			grid[rows][GRID_MAX_COLS - 1].dims.up = my_y;
			grid[rows][GRID_MAX_COLS - 1].dims.left = left_tile_x;
			grid[rows][GRID_MAX_COLS - 1].dims.right = grid[rows][GRID_MAX_COLS - 1].dims.left + 49.0f;
			grid[rows][GRID_MAX_COLS - 1].dims.down = grid[rows][GRID_MAX_COLS - 1].dims.up + 49.0f;

			switch (grid[rows][GRID_MAX_COLS - 1].type)
			{
			case tiles::dirt:
				grid[rows][GRID_MAX_COLS - 1].move_reduct = 0;
				break;

			case tiles::forest:
				grid[rows][GRID_MAX_COLS - 1].move_reduct = -0.5f;
				break;

			case tiles::lava:
				grid[rows][GRID_MAX_COLS - 1].move_reduct = -3.5f;
				break;

			case tiles::path:
				grid[rows][GRID_MAX_COLS - 1].move_reduct = +0.5f;
				break;

			case tiles::water:
				grid[rows][GRID_MAX_COLS - 1].move_reduct = -8.0f;
				break;
			}

			my_y += 50.0f;
		}
	}
}

tiles dll::GRID::get_type(int row, int col)const
{
	return grid[row][col].type;
}

void dll::GRID::set_type(tiles what_type, int row, int col)
{
	grid[row][col].type = what_type;
}

float dll::GRID::get_move_reduct(int row, int col)const
{
	return grid[row][col].move_reduct;
}

FRECT dll::GRID::get_dims(int row, int col)const
{
	return grid[row][col].dims;
}

void dll::GRID::move(dirs to_where, float gear)
{
	switch (to_where)
	{
	case dirs::up:
		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
			{
				grid[rows][cols].dims.up -= gear;
				grid[rows][cols].dims.down -= gear;
			}
		}
		if (grid[0][0].dims.down <= up_tile_y)
		{
			add_row(dirs::down);
			break;
		}
		break;

	case dirs::down:
		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
			{
				grid[rows][cols].dims.up += gear;
				grid[rows][cols].dims.down += gear;
			}
		}
		if (grid[GRID_MAX_ROWS - 1][0].dims.up >= down_tile_y)
		{
			add_row(dirs::up);
			break;
		}
		break;

	case dirs::left:
		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
			{
				grid[rows][cols].dims.left -= gear;
				grid[rows][cols].dims.right -= gear;
			}
		}
		if (grid[0][0].dims.right <= left_tile_x)
		{
			add_row(dirs::right);
			break;
		}
		break;

	case dirs::right:
		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
			{
				grid[rows][cols].dims.left += gear;
				grid[rows][cols].dims.right += gear;
			}
		}
		if (grid[0][GRID_MAX_COLS - 1].dims.left >= right_tile_x)
		{
			add_row(dirs::left);
			break;
		}
		break;

	case dirs::up_left:
		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
			{
				grid[rows][cols].dims.up -= gear;
				grid[rows][cols].dims.down -= gear;
				grid[rows][cols].dims.left -= gear;
				grid[rows][cols].dims.right -= gear;
			}
		}
		if (grid[0][0].dims.down <= up_tile_y)
		{
			add_row(dirs::down);
			break;
		}
		if (grid[0][0].dims.right <= left_tile_x)
		{
			add_row(dirs::right);
			break;
		}
		break;

	case dirs::up_right:
		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
			{
				grid[rows][cols].dims.up -= gear;
				grid[rows][cols].dims.down -= gear;
				grid[rows][cols].dims.left += gear;
				grid[rows][cols].dims.right += gear;
			}
		}
		if (grid[0][0].dims.down <= up_tile_y)
		{
			add_row(dirs::down);
			break;
		}
		if (grid[0][GRID_MAX_COLS - 1].dims.left >= right_tile_x)
		{
			add_row(dirs::left);
			break;
		}
		break;

	case dirs::down_left:
		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
			{
				grid[rows][cols].dims.up += gear;
				grid[rows][cols].dims.down += gear;
				grid[rows][cols].dims.left -= gear;
				grid[rows][cols].dims.right -= gear;
			}
		}
		if (grid[GRID_MAX_ROWS - 1][0].dims.up >= down_tile_y)
		{
			add_row(dirs::up);
			break;
		}
		if (grid[0][0].dims.right <= left_tile_x)
		{
			add_row(dirs::right);
			break;
		}
		break;

	case dirs::down_right:
		for (int rows = 0; rows < GRID_MAX_ROWS; ++rows)
		{
			for (int cols = 0; cols < GRID_MAX_COLS; ++cols)
			{
				grid[rows][cols].dims.up += gear;
				grid[rows][cols].dims.down += gear;
				grid[rows][cols].dims.left += gear;
				grid[rows][cols].dims.right += gear;
			}
		}
		if (grid[GRID_MAX_ROWS - 1][0].dims.up >= down_tile_y)
		{
			add_row(dirs::up);
			break;
		}
		if (grid[0][GRID_MAX_COLS - 1].dims.left >= right_tile_x)
		{
			add_row(dirs::left);
			break;
		}
		break;
	}
}

////////////////////////////////////////

// PIGS *******************************

dll::PIGS::PIGS(pigs _what_type, float _start_x, float _start_y) :PROTON(_start_x, _start_y)
{
	_type = _what_type;

	switch (_type)
	{
	case pigs::hero:
		new_dims(82.0f, 100.0f);
		speed = 1.5f;
		max_frames = 15;
		frame_delay = 4;
		break;

	case pigs::fly:
		new_dims(80.0f, 43.0f);
		speed = 0.9f;
		max_frames = 1;
		frame_delay = 30;
		break;

	case pigs::tough:
		new_dims(90.0f, 73.0f);
		speed = 1.5f;
		max_frames = 4;
		frame_delay = 12;
		break;

	case pigs::runner:
		new_dims(83.0f, 100.0f);
		speed = 0.7f;
		max_frames = 11;
		frame_delay = 5;
		break;

	case pigs::freak:
		new_dims(70.0f, 57.0f);
		speed = 0.6f;
		max_frames = 10;
		frame_delay = 5;
		break;
	}

	max_frame_delay = frame_delay;
	dir = dirs::stop;
}

pigs dll::PIGS::get_type()const
{
	return _type;
}
unsigned char dll::PIGS::Collision(FRECT myRect, FRECT ObstRect)
{
	unsigned char coll_detected = no_collision;

	if (!(myRect.left >= ObstRect.right || myRect.right <= ObstRect.left ||
		myRect.up >= ObstRect.down || myRect.down <= ObstRect.up))
	{
		if (myRect.left >= ObstRect.left && myRect.left <= ObstRect.right)coll_detected |= left_flag;
		else if (myRect.right >= ObstRect.left && myRect.right <= ObstRect.right)coll_detected |= right_flag;

		if (myRect.up >= ObstRect.up && myRect.up <= ObstRect.down)coll_detected |= up_flag;
		else if (myRect.down >= ObstRect.up && myRect.down <= ObstRect.up)coll_detected |= down_flag;
	}

	return coll_detected;
}

void dll::PIGS::SetPath(float _endx, float _endy)
{
	hor_dir = false;
	ver_dir = false;

	move_sx = start.x;
	move_sy = start.y;

	move_ex = _endx;
	move_ey = _endy;

	if (move_sx == move_ex || (move_ex > start.x && move_ex <= end.x))
	{
		ver_dir = true;
		return;
	}
	if (move_sy == move_ey || (move_ey > start.y && move_ey <= end.y))
	{
		hor_dir = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sx);
	intercept = start.y - start.x * slope;
}

int dll::PIGS::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++frame;
		if (frame > max_frames)frame = 0;
	}

	return frame;
}
void dll::PIGS::move(float gear)
{
	float my_speed = speed + gear / 10.0f;

	if (ver_dir)
	{
		if (move_sy > move_ey)
		{
			dir = dirs::up;
			if (start.y - my_speed >= sky)
			{
				start.y -= my_speed;
				set_edges();
				if (start.y <= move_ey) dir = dirs::stop;
				return;
			}
			else
			{
				dir = dirs::stop;
				return;
			}
		}
		else if (move_sy < move_ey)
		{
			dir = dirs::down;
			if (end.y + my_speed <= ground)
			{
				start.y += my_speed;
				set_edges();
				if (end.y >= move_ey) dir = dirs::stop;
				return;
			}
			else
			{
				dir = dirs::stop;
				return;
			}
		}
		else
		{
			dir = dirs::stop;
			return;
		}
	}
	if (hor_dir)
	{
		if (move_sx > move_ex)
		{
			dir = dirs::left;
			if (start.x - my_speed >= 0)
			{
				start.x -= my_speed;
				set_edges();
				if (start.x <= move_ex) dir = dirs::stop;
				return;
			}
			else
			{
				dir = dirs::stop;
				return;
			}
		}
		else if (move_sx < move_ex)
		{
			dir = dirs::right;
			if (end.x + my_speed <= scr_width)
			{
				start.x += my_speed;
				set_edges();
				if (end.x >= move_ex) dir = dirs::stop;
				return;
			}
			else
			{
				dir = dirs::stop;
				return;
			}
		}
		else
		{
			dir = dirs::stop;
			return;
		}
	}

	if (move_sx > move_ex)
	{
		dir = dirs::left;

		if (start.x - my_speed >= 0)
		{
			start.x -= my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (start.x <= move_ex) dir = dirs::stop;
			return;
		}
		else
		{
			dir = dirs::stop;
			return;
		}
	}
	else if (move_sx < move_ex)
	{
		dir = dirs::right;
		if (end.x + my_speed <= scr_width)
		{
			start.x += my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.x >= move_ex) dir = dirs::stop;
			return;
		}
		else
		{
			dir = dirs::stop;
			return;
		}
	}
	else
	{
		dir = dirs::stop;
		return;
	}
}

void dll::PIGS::Release()
{
	delete this;
}

void dll::PIGS::AIMove(BAG<FPOINT>& FoodBag, BAG<FPOINT>& ObstBag, FPOINT HeroPig, float game_speed)
{
	if (!FoodBag.empty())Sort(FoodBag, center);
	if (!ObstBag.empty())Sort(ObstBag, center);
	else move(game_speed);

	FRECT dummy{ ObstBag[0].x - 100.0f, ObstBag[0].y - 60.0f, ObstBag[0].x + 100.0f, ObstBag[0].y + 60.0f };
	FRECT myRect{ start.x,start.y,end.x,end.y };
	
	if (dir != dirs::stop)
	{
		switch (dir)
		{
		case dirs::up:
			myRect.up -= game_speed;
			myRect.down -= game_speed;
			switch (Collision(myRect, dummy))
			{
			case up_flag:
				if (dummy.left <= myRect.left)SetPath(0, ground);
				else SetPath(scr_width, ground);
				move(game_speed);
				break;

			case up_left_flag:
				SetPath(scr_width, ground);
				move(game_speed);
				break;

			case up_right_flag:
				SetPath(0, ground);
				move(game_speed);
				break;

			default: move(game_speed);
			}
			break;

		case dirs::down:
			myRect.up += game_speed;
			myRect.down += game_speed;
			switch (Collision(myRect, dummy))
			{
			case down_flag:
				if (dummy.left <= myRect.left)SetPath(0, sky);
				else SetPath(scr_width, sky);
				move(game_speed);
				break;

			case down_left_flag:
				SetPath(scr_width, sky);
				move(game_speed);
				break;

			case down_right_flag:
				SetPath(0, sky);
				move(game_speed);
				break;

			default: move(game_speed);
			}
			break;

		case dirs::left:
			myRect.left -= game_speed;
			myRect.right -= game_speed;
			switch (Collision(myRect, dummy))
			{
			case left_flag:
				if (dummy.up <= myRect.up)SetPath(scr_width, sky);
				else SetPath(scr_width, ground);
				move(game_speed);
				break;

			case up_left_flag:
				SetPath(scr_width, ground);
				move(game_speed);
				break;

			case down_left_flag:
				SetPath(scr_width, sky);
				move(game_speed);
				break;

			default: move(game_speed);
			}
			break;

		case dirs::right:
			myRect.left += game_speed;
			myRect.right += game_speed;
			switch (Collision(myRect, dummy))
			{
			case right_flag:
				if (dummy.up <= myRect.up)SetPath(0, sky);
				else SetPath(0, ground);
				move(game_speed);
				break;

			case up_right_flag:
				SetPath(0, ground);
				move(game_speed);
				break;

			case down_right_flag:
				SetPath(0, sky);
				move(game_speed);
				break;

			default: move(game_speed);
			}
			break;
		}
	}
	else
	{
		if (Distance(center, HeroPig) <= 200.0f)SetPath(HeroPig.x, HeroPig.y);
		else if (!FoodBag.empty())SetPath(FoodBag[0].x, FoodBag[0].y);
	}
}

dll::PIGS* dll::PIGS::create(pigs what_type, float start_x, float start_y)
{
	PIGS* ret{ nullptr };

	ret = new PIGS(what_type, start_x, start_y);

	return ret;
}

///////////////////////////////////////


//FUNCTIONS ***************************

bool dll::Intersect(FRECT first, FRECT second)
{
	if (!(first.left >= second.right || first.right <= second.left ||
		first.up >= second.down || first.down <= second.up))return true;
	return false;
}
bool dll::Intersect(FPOINT first, FPOINT second, float first_x_rad, float second_x_rad,
	float first_y_rad, float second_y_rad)
{
	if ((abs(second.x - first.x) <= first_x_rad + second_x_rad) && (abs(second.y - first.y) <= first_y_rad + second_y_rad))
		return true;
	return false;
}

float dll::Distance(FPOINT first, FPOINT second)
{
	float a = (float)(pow(abs(second.x - first.x), 2));
	float b = (float)(pow(abs(second.y - first.y), 2));

	return (float)(sqrt(a + b));
}
void dll::Sort(BAG<FPOINT>& container, FPOINT target)
{
	if (container.size() < 2)return;
	bool is_ok = false;
	while (!is_ok)
	{
		is_ok = true;
		for (size_t count = 0; count < container.size() - 1; ++count)
		{
			if (Distance(container[count], target) > Distance(container[count + 1], target))
			{
				FPOINT temp = container[count];
				container[count] = container[count + 1];
				container[count + 1] = temp;
				is_ok = false;
			}
		}
	}
}