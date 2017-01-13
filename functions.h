#include <cstdlib>
#include <ctime>
#include <iostream>
#include "declarations.h"
#include <cstring>
#include <cstdlib>
#include <windows.h>

using namespace std;

void generate_matrix(int numberOfBombs)
{
    int x, y, rx, ry, i;
    int counter;

    counter = numberOfBombs;
    srand(time(NULL));

    while(counter)
    {
        x = rand() % mat.n + 1;
        y = rand() % mat.m + 1;

        if(mat.values[x][y] != -1)
        {
            mat.values[x][y] = -1;
            for(i = 0; i < 8; i++)
            {
                rx = x + dx[i];
                ry = y + dy[i];
                if(rx >= 1 && rx <= mat.n && ry >= 1 && ry <= mat.m && mat.values[rx][ry] != -1)
                    mat.values[rx][ry]++;
            }
            counter--;
        }
    }
}

void bomb_pressed_first(int row, int column)
{
    int rowNeighbour, columnNeighbour;
    int i, counter, x, y, rx, ry;

    counter = 1;
    mat.values[row][column] = 0;

    for(i = 0; i < 8; i++)
    {
        rowNeighbour = row + dx[i];
        columnNeighbour = column + dy[i];

        if(mat.values[rowNeighbour][columnNeighbour] != -1)
            mat.values[rowNeighbour][columnNeighbour]--;
        else
            mat.values[row][column]++;
    }

    while(counter)
    {
        x = rand() % mat.n + 1;
        y = rand() % mat.m + 1;

        if(mat.values[x][y] != -1)
        {
            mat.values[x][y] = -1;
            cout<<x<<" "<<y;
            for(i = 0; i < 8; i++)
            {
                rx = x + dx[i];
                ry = y + dy[i];
                if(rx >= 1 && rx <= mat.n && ry >= 1 && ry <= mat.m && mat.values[rx][ry] != -1)
                    mat.values[rx][ry]++;
            }
            counter--;
        }
    }
}

void cls()
 {
    COORD coordScreen = { 0, 0 };
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    GetConsoleScreenBufferInfo( hConsole, &csbi );

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
       dwConSize, coordScreen, &cCharsWritten );

    GetConsoleScreenBufferInfo( hConsole, &csbi );

    FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
       dwConSize, coordScreen, &cCharsWritten );

    SetConsoleCursorPosition( hConsole, coordScreen );
    return;
 }

void click_in_game(position &input)
{
    HANDLE hIn;
    HANDLE hOut;
    INPUT_RECORD InRec;
    DWORD NumRead;
    int x, y, aux;

    hIn = GetStdHandle(STD_INPUT_HANDLE);
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true)
    {
        ReadConsoleInput(hIn,
                         &InRec,
                         1,
                         &NumRead);

        if (InRec.EventType == MOUSE_EVENT)
        {

            if(InRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            {
                x = InRec.Event.MouseEvent.dwMousePosition.X ;
                y = InRec.Event.MouseEvent.dwMousePosition.Y ;

                if(y == mat.n + 3 && x == 0)
                {
                    input.row = -1;
                    input.column = -1;
                    break;
                }

                aux = x;
                x = y;
                y = aux;

                if(y % 2 == 0)
                {
                    y /= 2;
                    x = x - 1;
                    y++;
                    if(x >= 1 && x <= mat.n && y >= 1 && y <= mat.m)
                    {
                        input.row = x;
                        input.column = y;
                        input.flag = 0;
                        break;
                    }
                }
            }

        else
        {
            if(InRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
            {
                x = InRec.Event.MouseEvent.dwMousePosition.X ;
                y = InRec.Event.MouseEvent.dwMousePosition.Y ;

                aux = x;
                x = y;
                y = aux;

                if(y % 2 == 0)
                {
                    y /= 2;
                    x = x - 1;
                    y++;
                    if(x >= 1 && x <= mat.n && y >= 1 && y <= mat.m)
                    {
                        input.row = x;
                        input.column = y;
                        input.flag = 1;
                        break;
                    }
                }
            }
        }
    }
}
}

int click_in_menu()
{
    HANDLE hIn;
    HANDLE hOut;
    INPUT_RECORD InRec;
    DWORD NumRead;
    int x, y, i;
    int yPositions[] = {19, 21, 23, 25, 27};


    hIn = GetStdHandle(STD_INPUT_HANDLE);
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true)
    {
        ReadConsoleInput(hIn,
                         &InRec,
                         1,
                         &NumRead);

        if (InRec.EventType == MOUSE_EVENT)
        {

            if(InRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            {
                x = InRec.Event.MouseEvent.dwMousePosition.X ;
                y = InRec.Event.MouseEvent.dwMousePosition.Y ;
                if(x == 88)
                    for(i = 0; i < 5; i++)
                        if(y == yPositions[i])
                            return y;
            }
        }
    }
}

void in_game_menu();

int input_move(int &blocksToWin, position input)
{
    Queue C[10201], v, z;
    int p, u, i;
    int line, column;

    line = input.row;
    column = input.column;

    if(line == -1)
    {
        in_game_menu();
        exit(0);
    }

    if(input.flag == 1 && printed[line][column] == 0)
        if(flagged[line][column] == 0)
        {
            flagged[line][column] = 1;
            minesLeft--;
        }
        else
        {
            flagged[line][column] = 0;
            minesLeft++;
        }
    else
        if(printed[line][column] == 0 && flagged[line][column] == 0)
            if(mat.values[line][column] == -1 && bombFirst == true)
                return -1;
            else
            {
                if(mat.values[line][column] == -1 && bombFirst == false)
                {
                    bomb_pressed_first(line, column);
                    bombFirst = true;
                }
                if(mat.values[line][column] != 0)
                {
                    printed[line][column] = 1;
                    blocksToWin--;
                    bombFirst = true;
                    return 1;
                }
                else
                {
                    bombFirst = true;

                    p = u = 1;
                    C[p].row = line;
                    C[p].column = column;

                    while(p <= u)
                    {
                        v = C[p];
                        p++;
                        printed[v.row][v.column] = 1;
                        blocksToWin--;
                        for(i = 0; i < 8; i++)
                        {
                            z.row = v.row + dx[i];
                            z.column = v.column + dy[i];

                            if(z.row >= 1 && z.row <= mat.n && z.column >= 1 && z.column <= mat.m && printed[z.row][z.column] == 0)
                                if(mat.values[z.row][z.column] != 0)
                                {
                                    printed[z.row][z.column] = 1;
                                    blocksToWin--;
                                }
                                else
                                {
                                    C[++u].row = z.row;
                                    C[u].column = z.column;
                                    printed[z.row][z.column] = 1;
                                }
                        }
                    }
                    return 1;
                }
            }
    return 1;
}


void print_matrix()
{
    int i, j;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for(i = 1; i <= mat.n; i++)
    {
        for(j = 1; j <= mat.m; j++)
            if(flagged[i][j])
                cout<<'F'<<" ";
            else
                if(printed[i][j])
                    if(mat.values[i][j] == 0)
                        cout<<0<<" ";
                    else
                    {
                        switch((int)mat.values[i][j])
                        {
                            case -1:
                                if(printBombs == true)
                                    cout<<'B'<<" ";
                                break;

                            case 1:
                                SetConsoleTextAttribute(hConsole, 9);
                                cout<<1<<" ";
                                break;

                            case 2:
                                SetConsoleTextAttribute(hConsole, 10);
                                cout<<2<<" ";
                                break;

                            case 3:
                                SetConsoleTextAttribute(hConsole, 12);
                                cout<<3<<" ";
                                break;

                            case 4:
                                SetConsoleTextAttribute(hConsole, 1);
                                cout<<4<<" ";
                                break;

                            case 5:
                                SetConsoleTextAttribute(hConsole, 4);
                                cout<<5<<" ";
                                break;

                            case 6:
                                SetConsoleTextAttribute(hConsole, 3);
                                cout<<6<<" ";
                                break;

                            case 7:
                                SetConsoleTextAttribute(hConsole, 14);
                                cout<<7<<" ";
                                break;

                            case 8:
                                SetConsoleTextAttribute(hConsole, 8);
                                cout<<8<<" ";
                                break;
                        }
                        SetConsoleTextAttribute(hConsole, 15);
                    }
                else
                    cout<<'+'<<" ";
        cout<<'\n';
    }
    cout<<"\n\xDB Meniu";
}

void make_all_printed()
{
    int i, j;
    for(i = 1; i <= mat.n; i++)
        for(j = 1; j <= mat.m; j++)
        {
            printed[i][j] = 1;
            if(flagged[i][j] == true)
                flagged[i][j] = false;
        }
}

void run_game()
{
    int ok, result, blocksToWin;
    ok = 1;
    generate_matrix(bombs);
    blocksToWin = mat.n * mat.m - bombs;
    minesLeft = bombs;

    while(ok && blocksToWin != 0)
    {
        cls();
        cout<<"Bombe negasite: "<<minesLeft<<"\n\n";
        print_matrix();
        click_in_game(input);
        result = input_move(blocksToWin, input);
        if(result == -1)
        {
            cls();
            cout<<"GAME OVER! \n\n";
            ok = 0;
            printBombs = true;
            make_all_printed();
            print_matrix();
            COORD CursorPosition;
            CursorPosition.X = 0;
            CursorPosition.Y = mat.n + 3;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),CursorPosition);
            cout<<"Apasa ENTER pentru a reveni la meniu";
            cin.get();
        }
    }
    if(blocksToWin == 0)
    {
        cls();
        cout<<"YOU ARE A WINNER \n\n";
        printBombs = true;
        make_all_printed();
        print_matrix();
        COORD CursorPosition;
        CursorPosition.X = 0;
        CursorPosition.Y = mat.n + 3;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),CursorPosition);
        cout<<"Apasa ENTER pentru a reveni la meniu";
    }
}

void custom_game()
{
    int i;
    char heightInput[10], lengthInput[10], bombsInput[10];

    for(i = 0; i < 10; i++)
        heightInput[i] = lengthInput[i] = bombsInput[i] = NULL;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cls();

    cout<<"Introduceti inaltimea mapei: ";
    cin>>heightInput;

    if(cin.get() !='\n')
    {
        custom_game();
        exit(0);
    }

    for(i = 0; i < strlen(heightInput); i++)
        if(heightInput[i] < '0' || heightInput[i] > '9')
        {
            custom_game();
            exit(0);
        }

    mat.n = atoi(heightInput);

    if(mat.n < 9) mat.n = 9;
    if(mat.n > 24) mat.n = 24;

    cout<<"Introduceti latimea mapei: ";
    cin>>lengthInput;

    if(cin.get() !='\n')
    {
        custom_game();
        exit(0);
    }

    for(i = 0; i < strlen(lengthInput); i++)
        if(lengthInput[i] < '0' || lengthInput[i] > '9')
        {
            custom_game();
            exit(0);
        }

    mat.m = atoi(lengthInput);

    if(mat.m < 9) mat.m = 9;
    if(mat.m > 30) mat.m = 30;

    cout<<"Introduceti numarul de bombe: ";
    cin>>bombsInput;

    if(cin.get() !='\n')
    {
        custom_game();
        exit(0);
    }

    for(i = 0; i < strlen(bombsInput); i++)
        if(bombsInput[i] < '0' || bombsInput[i] > '9')
        {
            custom_game();
            exit(0);
        }

    bombs = atoi(bombsInput);

    if(bombs < 10) bombs = 10;
    if(bombs > (mat.n - 1) * (mat.m-1))
        bombs = (mat.n - 1) * (mat.m - 1);
    run_game();
}

void initialize()
{
    int i, j;
    mat.n = mat.m = bombs = bombFirst = printBombs = 0;
    for(i = 1; i <= 100; i++)
        for(j = 1; j <= 100; j++)
            mat.values[i][j] = flagged[i][j] = printed[i][j] = 0;
}

void in_game_menu()
{
    initialize();
    cls();

    int k, option, i;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


                       cout<<"                                       @@@     .@@@  #@:  @@@   :@,  @@@@@@  .@@@@` @@    @@.   @@  @@@@@@  @@@@@@  @@@@@#   @@@@@@  @@@@@@ \n";
                       cout<<"                                      `@@@;    #@@@  #@:  @@@   :@,  @@@@@@  @@@@@: @@    @@;   @@  @@@@@@  @@@@@@  @@@@@@;  @@@@@@  @@@@@@@ \n";
                       cout<<"                                      `@@@#    @@@@  #@:  @@@:  :@,  @@@@@@ `@@::@: @@.  `@@#   @@  @@@@@@  @@@@@@  @@@@@@@  @@@@@@  @@:,@@@ \n";
                       cout<<"                                      `@@@@    @@@@  #@:  @#@@  :@,  @@     :@@   ` #@'  ,@@@   @@  @@      @@      @@  .@@  @@      @@   @@ \n";
                       cout<<"                                      `@@@@   `@+@@  #@:  @#@@  :@,  @@     '@+     ;@#  +@@@   @@  @@      @@      @@   @@  @@      @@   @@. \n";
                       cout<<"                                      `@@#@`  ;@.@@  #@:  @#@@. :@,  @@     ;@@     `@@  @@@@  .@'  @@      @@      @@   @@` @@      @@   @@` \n";
                       cout<<"                                      `@@;@;  #@ @@  #@:  @#'@# :@,  @@     .@@;     @@  @@@@  ;@,  @@      @@      @@   @@  @@      @@   @@ \n";
                       cout<<"                                      `@@`@@  @@ @@  #@:  @# @@ :@,  @@+++   @@@#    @@  @@#@` #@   @@+++`  @@+++,  @@   @@  @@+++,  @@  :@@ \n";
                       cout<<"                                      `@@ @@  @@ @@  #@:  @# @@ :@,  @@@@@`  @@@@@   @@  @#;@: @@   @@@@@,  @@@@@+  @@  #@@  @@@@@+  @@@@@@+ \n";
                       cout<<"                                      `@@ @@ `@' @@  #@:  @# @@':@,  @@@@@`   @@@@.  @@``@;.@# @@   @@@@@,  @@@@@'  @@@@@@#  @@@@@+  @@@@@+ \n";
                       cout<<"                                      `@@ #@`'@. @@  #@:  @# ,@@:@,  @@        #@@@  +@,.@` @@ @@   @@      @@      @@@@@@   @@      @@@@@@ \n";
                       cout<<"                                      `@@ ;@'@@  @@  #@:  @#  @@:@,  @@         @@@  :@';@  @@ @#   @@      @@      @@@@@`   @@      @@  @@. \n";
                       cout<<"                                      `@@ `@@@@  @@  #@:  @#  @@;@,  @@          @@  `@##@  @@ @;   @@      @@      @@       @@      @@  +@@ \n";
                       cout<<"                                      `@@  @@@@  @@  #@:  @#  '@@@,  @@          @@   @@@@  @@,@.   @@      @@      @@       @@      @@  `@@ \n";
                       cout<<"                                      `@@  @@@'  @@  #@:  @#  `@@@,  @@     #   `@@   @@@@  #@+@    @@      @@      @@       @@      @@   @@ \n";
                       cout<<"                                      `@@  @@@.  @@  #@:  @#   @@@,  @@++++ @@` @@@   @@@@  ;@@@    @@++++  @@++++  @@       @@++++  @@   @@, \n";
                       cout<<"                                      `@@  '@@   @@  #@:  @#   #@@,  @@@@@@ @@@@@@.   @@@'  .@@@    @@@@@@  @@@@@@  @@       @@@@@@  @@   #@# \n";
                       cout<<"                                      `@@  `@@   @@  #@:  @#   .@@.  @@@@@@ :@@@@@    +@@,   @@@    @@@@@@  @@@@@@  @@       @@@@@@  @@   ,@@ \n";

    SetConsoleTextAttribute(hConsole, 2);
    cout<<"\n\t\t\t\t\t\t\t\t\t\t\t\xDB USOR \n";

    SetConsoleTextAttribute(hConsole, 14);
    cout<<"\n\t\t\t\t\t\t\t\t\t\t\t\xDB MEDIU \n";

    SetConsoleTextAttribute(hConsole, 12);
    cout<<"\n\t\t\t\t\t\t\t\t\t\t\t\xDB EXPERT \n";

    SetConsoleTextAttribute(hConsole, 13);
    cout<<"\n\t\t\t\t\t\t\t\t\t\t\t\xDB CUSTOM \n";

    SetConsoleTextAttribute(hConsole, 15);
    cout<<"\n\t\t\t\t\t\t\t\t\t\t\t\xDB EXIT \n";

    cout<<"\n\t\t\t\t\t\t\t\t\tClick pe dreptunghi pentru a selecta ";

    option = click_in_menu();

    switch(option)
    {
        case 19:
            mat.n = mat.m = 9;
            bombs = 10;
            run_game();
            in_game_menu();
        break;

        case 21:
            mat.n = mat.m = 16;
            bombs = 40;
            run_game();
            in_game_menu();
        break;

        case 23:
            mat.n = 30;
            mat.m = 16;
            bombs = 99;
            run_game();
            in_game_menu();
        break;

        case 25:
            custom_game();
            in_game_menu();
            break;

        case 27:
            break;
    }
}
