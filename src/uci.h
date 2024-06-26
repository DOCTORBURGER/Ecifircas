#pragma once

#include <iostream>
#include <string>

#include "types.h"

namespace Ecifircas {
	class UCI {
	public:
		void loop();

		static std::string	move(Move m);
		static Move			to_move(std::string& str);

	private:
		void go(std::istringstream& iss);
		void position(std::istringstream& iss);
	};
}