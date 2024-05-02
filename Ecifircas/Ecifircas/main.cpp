#include <iostream>

#include "bitboard.h"
#include "uci.h"

using namespace Ecifircas;

int main()
{
    initialize_bitboards();
    
    UCI uci;

    uci.loop();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
