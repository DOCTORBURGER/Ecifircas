#pragma once

#include "types.h"

namespace Ecifircas
{
	extern long long Nodes;
	void perft_driver(int depth);
	long long perft_test(int depth, bool output);
	void perft_test_suite();
}
