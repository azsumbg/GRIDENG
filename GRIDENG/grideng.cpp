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

void add_col(dirs where);

tiles dll::GRID::get_type(int row, int col)const
{
	return grid[row][col].type;
}

float dll::GRID::get_move_reduct(int row, int col)const
{
	return grid[row][col].move_reduct;
}

FRECT dll::GRID::get_dims(int row, int col)const
{
	return grid[row][col].dims;
}

void move(dirs to_where, float gear);

////////////////////////////////////////
