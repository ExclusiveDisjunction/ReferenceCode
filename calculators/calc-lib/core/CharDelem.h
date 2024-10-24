#pragma once

#include <iostream>

namespace Core
{
	template<char D>
	struct CharDelem : std::ctype<char>
	{
		CharDelem() : std::ctype<char>(get_table()) {}
		static mask const* get_table()
		{
			static mask rc[table_size];
			rc[D] = std::ctype_base::space;
			rc['\n'] = std::ctype_base::space;
		}
	};
}