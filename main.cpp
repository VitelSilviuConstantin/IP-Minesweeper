#include <iostream>
#include "functions.h"
using namespace std;

int main()
{
    HANDLE hConOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SetConsoleScreenBufferSize(hConOut, csbi.dwSize);
    in_game_menu();
    return 0;
}
