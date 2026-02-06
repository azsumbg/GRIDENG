#pragma once

#ifdef GRIDENG_EXPORTS
#define GRIDENG_API __declspec(dllexport)
#else 
#define GRIDENG_API __declspec(dllimport)
#endif

#include <random>

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

constexpr float up_tile_y{ -100.0f };
constexpr float down_tile_y{ 850.0f };

constexpr float left_tile_x{ -100.0f };
constexpr float right_tile_x{ scr_width + 100.0f };

constexpr int GRID_MAX_COLS{ 16 };
constexpr int GRID_MAX_ROWS{ 20 };

constexpr int BAG_OK = 4000;
constexpr int BAG_BAD_PTR = 4001;
constexpr int BAG_BAD_INDEX = 4002;
constexpr int BAG_UNKNOWN_ERR = 4003;

enum class dirs { up_left = 0, up = 1, up_right = 2, right = 3, down_right = 4, down = 5, down_left = 6, left = 7 };
enum class tiles { dirt = 0, forest = 1, lava = 2, path = 3, water = 4 };

struct GRIDENG_API FPOINT
{
	float x{ 0 };
	float y{ 0 };
};
struct GRIDENG_API FRECT
{
	float left{ 0 };
	float up{ 0 };
	float right{ 0 };
	float down{ 0 };
};
struct GRIDENG_API TILE
{
	tiles type{ tiles::dirt };
	
	FRECT dims{};
	
	float move_reduct{ 0 };
};

namespace dll
{
	template<typename T> class BAG
	{
	private:
		T* m_ptr{ nullptr };
		size_t max_size{ 0 };
		size_t next_pos{ 0 };
		bool valid = false;

	public:
		BAG() :max_size{ 1 }, m_ptr{ reinterpret_cast<T>(calloc(1, sizeof(T))) } {};
		BAG(size_t capacity) :max_size{ capacity }, m_ptr{ reinterpret_cast<T>(calloc(capacity, sizeof(T))) } {};
		BAG(BAG& other)
		{
			max_size = other.max_size;
			m_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
			for (size_t count = 0; count < other.next_pos; ++count)m_ptr[count] = other.m_ptr[count];
			next_pos = other.next_pos;
			valid = other.valid;
		}
		BAG(BAG&& other)
		{
			m_ptr = other.m_ptr;
			max_size = other.max_size;
			next_pos = other.next_pos;
			valid = other.valid;
			other.m_ptr = nullptr;
		}
		~BAG()
		{
			free(m_ptr);
			m_ptr = nullptr;
		}

		int push_back(T element)
		{
			if (!m_ptr)
			{
				valid = false;
				return BAG_BAD_PTR;
			}
			else
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = element;
					++next_pos;
					valid = true;
					return BAG_OK;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
					if (!mPtr)return BAG_BAD_PTR;
					else
					{
						m_ptr[next_pos] = element;
						++next_pos;
						valid = true;
						return BAG_OK;
					}
				}
			}

			return BAG_UNKNOWN_ERR;
		}
		int push_back(T* element)
		{
			if (!m_ptr)
			{
				valid = false;
				return BAG_BAD_PTR;
			}
			else
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = (*element);
					++next_pos;
					valid = true;
					return BAG_OK;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
					if (!mPtr)return BAG_BAD_PTR;
					else
					{
						m_ptr[next_pos] = *element;
						++next_pos;
						valid = true;
						return BAG_OK;
					}
				}
			}

			return BAG_UNKNOWN_ERR;
		}

		int push_front(T element)
		{
			if (!m_ptr)
			{
				valid = false;
				return BAG_BAD_PTR;
			}
			else
			{
				if (next_pos + 1 <= max_size)
				{
					for (size_t count = next_pos; count > 0; --count)m_ptr[count] = m_ptr[count - 1];
					m_ptr[0] = element;
					++next_pos;
					valid = true;
					return BAG_OK;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
					if (!m_ptr)
					{
						valid = false;
						return BAG_BAD_PTR;
					}
					else
					{
						for (size_t count = next_pos; count > 0; --count)m_ptr[count] = m_ptr[count - 1];
						m_ptr[0] = element;
						++next_pos;
						valid = true;
						return BAG_OK;
					}
				}
			}

			return BAG_UNKNOWN_ERR;
		}
		int push_front(T* element)
		{
			if (!m_ptr)
			{
				valid = false;
				return BAG_BAD_PTR;
			}
			else
			{
				if (next_pos + 1 <= max_size)
				{
					for (size_t count = next_pos; count > 0; --count)m_ptr[count] = m_ptr[count - 1];
					m_ptr[0] = *element;
					++next_pos;
					valid = true;
					return BAG_OK;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
					if (!m_ptr)
					{
						valid = false;
						return BAG_BAD_PTR;
					}
					else
					{
						for (size_t count = next_pos; count > 0; --count)m_ptr[count] = m_ptr[count - 1];
						m_ptr[0] = *element;
						++next_pos;
						valid = true;
						return BAG_OK;
					}
				}
			}

			return BAG_UNKNOWN_ERR;
		}




	};


	class GRIDENG_API RANDIT
	{
	private:
		std::mt19937* twister{ nullptr };

	public:
		RANDIT();
		~RANDIT();

		int operator ()(int min, int max);
		float operator ()(float min, float max);
	};

	class GRIDENG_API PROTON
	{
	protected:
		float _width{ 0 };
		float _height{ 0 };

	public:
		FPOINT start{};
		FPOINT end{};
		FPOINT center{};
		float x_radius{ 0 };
		float y_radius{ 0 };

		PROTON();
		PROTON(float firstx, float firsty);
		PROTON(float firstx, float firsty, float firstw, float firsth);

		virtual ~PROTON() {};

		void set_edges();
		void new_dims(float new_width, float new_height);
		void set_width(float new_width);
		void set_height(float new_height);

		float get_width()const;
		float get_height()const;
	};

	class GRIDENG_API GRID
	{
	private:
		TILE grid[GRID_MAX_ROWS][GRID_MAX_COLS]{};

		RANDIT RandIt{};

		void add_row(dirs where);

		void add_col(dirs where);

	public:
		GRID();

		tiles get_type(int row, int col)const;

		void set_type(tiles what_type, int row, int col);

		float get_move_reduct(int row, int col)const;

		FRECT get_dims(int row, int col)const;

		void move(dirs to_where, float gear);
	};




}