#include <iostream>
#include "Map.h"
#include "Autoplay.h"
using namespace std;

// Hàm khởi tạo game
void game (bool &replay)
{
    Minesweeper test;
    int n = 0;
    test.options(n);
    if (1 <= n && n <= 4)
    {
        if (n == 4)
        {
            test.creating_map();
            test.processing_bomb();
            Autoplay(test, replay);
        }
        else
        {
            test.creating_map();
            test.processing_bomb();
            Map(test, replay);
        }
    }
}

int main(int argc, char *argv[])
{
	bool replay = true;
    // Trong khi replay đúng thì chạy hàm
    while (replay == true)
    {
        game(replay);
    }
	return 0;
}
