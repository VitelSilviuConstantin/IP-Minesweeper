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

void in_game_menu();

int input_move(int &blocksToWin)
{
    Queue C[10201], v, z;
    int p, u, i;
    char flag, lineInput[10], columnInput[10];
    int line, column;

    for(i = 0; i < 10; i++)
        lineInput[i] = columnInput[i] = NULL;

    cout<<"\nIntroduceti linie: ";
    cin>>lineInput;

    if(cin.get() !='\n')
    {
        while(cin.get()!='\n');
        return 1;
    }

    for(i = 0; i < strlen(lineInput); i++)
        if(lineInput[i] < '0' || lineInput[i] > '9')
            return 1;

    line = atoi(lineInput);
    if(line < 1 || line > mat.n) return 1;


    cout<<"Introduceti coloana: ";
    cin>>columnInput;

    if(cin.get() !='\n')
    {
        while(cin.get()!='\n');
        return 1;
    }

    for(i = 0; i < strlen(columnInput); i++)
        if(columnInput[i] < '0' || lineInput[i] > '9')
            return 1;

    column = atoi(columnInput);
    if(column < 1 || column > mat.m)
        return 1;

    if(flagged[line][column] == 1)
        cout<<"Stergeti flag? Y/N / M pentru meniu: ";
    else
        cout<<"Puneti flag? Y/N / M pentru meniu: ";
    cin>>flag;

    flag = toupper(flag);

    if(flag != 'Y' && flag != 'N' && flag != 'M')
        return 1;

    if(flag == 'M')
    {
        in_game_menu();
        exit(0);
    }

    if(flag == 'Y' && printed[line][column] == 0)
    {
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
        return 1;
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

    if(mat.m > 9)
    {
        if(mat.n > 9) cout<<"    ";
        else cout<<"   ";
        for(i = 1; i <= mat.m; i++)
            if(i / 10 == 0) cout<<"  ";
            else cout<<i / 10<<" ";
        cout<<'\n';
    }

    if(mat.n > 9) cout<<"    ";
        else cout<<"   ";
    for(i = 1; i <= mat.m; i++)
        cout<<i % 10<<" ";
    cout<<'\n';

    if(mat.n > 9) cout<<"    ";
        else cout<<"   ";

    for(i = 1; i<=mat.m; i++)
        cout<<'-'<<" ";
    cout<<'\n';

    for(i = 1; i <= mat.n; i++)
    {
        if(mat.n > 9 && i < 10) cout<<" ";
        cout<<i<<"| ";
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
    system("cls");

    while(ok && blocksToWin != 0)
    {
        cout<<"Mines left: "<<minesLeft<<"\n\n";
        print_matrix();
        result = input_move(blocksToWin);
        system("cls");
        if(result == -1)
        {
            cout<<"GAME OVER! \n\n";
            ok = 0;
            printBombs = true;
            make_all_printed();
            print_matrix();
            system("pause");
        }
    }
    if(blocksToWin == 0)
    {
        cout<<"YOU ARE A WINNER \n\n";
        printBombs = true;
        make_all_printed();
        print_matrix();
        system("pause");
    }
}

void custom_game()
{
    int i;
    char heightInput[10], lengthInput[10], bombsInput[10];

    for(i = 0; i < 10; i++)
        heightInput[i] = lengthInput[i] = bombsInput[i] = NULL;

    system("cls");
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
    system("cls");
    initialize();

    int k, option, i;
    char optionInput[10];

    for(i = 0; i < 10; i++)
        optionInput[i] = NULL;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

                       cout<<" @@@     .@@@  #@:  @@@   :@,  @@@@@@  .@@@@` @@    @@.   @@  @@@@@@  @@@@@@  @@@@@#   @@@@@@  @@@@@@ \n";
                       cout<<"`@@@;    #@@@  #@:  @@@   :@,  @@@@@@  @@@@@: @@    @@;   @@  @@@@@@  @@@@@@  @@@@@@;  @@@@@@  @@@@@@@ \n";
                       cout<<"`@@@#    @@@@  #@:  @@@:  :@,  @@@@@@ `@@::@: @@.  `@@#   @@  @@@@@@  @@@@@@  @@@@@@@  @@@@@@  @@:,@@@ \n";
                       cout<<"`@@@@    @@@@  #@:  @#@@  :@,  @@     :@@   ` #@'  ,@@@   @@  @@      @@      @@  .@@  @@      @@   @@ \n";
                       cout<<"`@@@@   `@+@@  #@:  @#@@  :@,  @@     '@+     ;@#  +@@@   @@  @@      @@      @@   @@  @@      @@   @@. \n";
                       cout<<"`@@#@`  ;@.@@  #@:  @#@@. :@,  @@     ;@@     `@@  @@@@  .@'  @@      @@      @@   @@` @@      @@   @@` \n";
                       cout<<"`@@;@;  #@ @@  #@:  @#'@# :@,  @@     .@@;     @@  @@@@  ;@,  @@      @@      @@   @@  @@      @@   @@ \n";
                       cout<<"`@@`@@  @@ @@  #@:  @# @@ :@,  @@+++   @@@#    @@  @@#@` #@   @@+++`  @@+++,  @@   @@  @@+++,  @@  :@@ \n";
                       cout<<"`@@ @@  @@ @@  #@:  @# @@ :@,  @@@@@`  @@@@@   @@  @#;@: @@   @@@@@,  @@@@@+  @@  #@@  @@@@@+  @@@@@@+ \n";
                       cout<<"`@@ @@ `@' @@  #@:  @# @@':@,  @@@@@`   @@@@.  @@``@;.@# @@   @@@@@,  @@@@@'  @@@@@@#  @@@@@+  @@@@@+ \n";
                       cout<<"`@@ #@`'@. @@  #@:  @# ,@@:@,  @@        #@@@  +@,.@` @@ @@   @@      @@      @@@@@@   @@      @@@@@@ \n";
                       cout<<"`@@ ;@'@@  @@  #@:  @#  @@:@,  @@         @@@  :@';@  @@ @#   @@      @@      @@@@@`   @@      @@  @@. \n";
                       cout<<"`@@ `@@@@  @@  #@:  @#  @@;@,  @@          @@  `@##@  @@ @;   @@      @@      @@       @@      @@  +@@ \n";
                       cout<<"`@@  @@@@  @@  #@:  @#  '@@@,  @@          @@   @@@@  @@,@.   @@      @@      @@       @@      @@  `@@ \n";
                       cout<<"`@@  @@@'  @@  #@:  @#  `@@@,  @@     #   `@@   @@@@  #@+@    @@      @@      @@       @@      @@   @@ \n";
                       cout<<"`@@  @@@.  @@  #@:  @#   @@@,  @@++++ @@` @@@   @@@@  ;@@@    @@++++  @@++++  @@       @@++++  @@   @@, \n";
                       cout<<"`@@  '@@   @@  #@:  @#   #@@,  @@@@@@ @@@@@@.   @@@'  .@@@    @@@@@@  @@@@@@  @@       @@@@@@  @@   #@# \n";
                       cout<<"`@@  `@@   @@  #@:  @#   .@@.  @@@@@@ :@@@@@    +@@,   @@@    @@@@@@  @@@@@@  @@       @@@@@@  @@   ,@@ \n";

    SetConsoleTextAttribute(hConsole, 2);
    cout<<"\n\t1. USOR \n";

    SetConsoleTextAttribute(hConsole, 14);
    cout<<"\n\t2. MEDIU \n";

    SetConsoleTextAttribute(hConsole, 12);
    cout<<"\n\t3. EXPERT \n";

    SetConsoleTextAttribute(hConsole, 13);
    cout<<"\n\t4. CUSTOM \n";

    SetConsoleTextAttribute(hConsole, 15);
    cout<<"\n\t5. EXIT \n";

    cout<<"\nSelecteaza optiunea: ";

    cin>>optionInput;

    if(cin.get() !='\n')
    {
        while(cin.get()!='\n');
        option = -1;
    }

    for(i = 0; i < strlen(optionInput); i++)
        if(optionInput[i] < '1' || optionInput[i] > '9')
        {
            option = -1;
            break;
        }

    if(option != -1)
        option = atoi(optionInput);

    switch(option)
    {
        case 1:
            mat.n = mat.m = 9;
            bombs = 10;
            run_game();
            in_game_menu();
        break;

        case 2:
            mat.n = mat.m = 16;
            bombs = 40;
            run_game();
            in_game_menu();
        break;

        case 3:
            mat.n = 30;
            mat.m = 16;
            bombs = 99;
            run_game();
            in_game_menu();
        break;

        case 4:
            custom_game();
            in_game_menu();
            break;

        case 5:
            break;

        default:
            cout<<"Optiune incorecta! \n";
            system("pause");
            in_game_menu();
    }
}
