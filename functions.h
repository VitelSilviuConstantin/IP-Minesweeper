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

int input_move(int &blocksToWin)
{
    Queue C[10201], v, z;
    int p, u, i;
    char flag;
    int line, column;
    cout<<"Introduceti mutarea:";
    cin>>line>>column;
    if(flagged[line][column] == 1)
        cout<<"Stergeti flag? Y/N ";
    else
        cout<<"Puneti flag? Y/N ";
    cin>>flag;

    flag = toupper(flag);

    if(flag == 'Y' && printed[line][column] == 0)
    {
        if(flagged[line][column] == 0)
            flagged[line][column] = 1;
        else
            flagged[line][column] = 0;
        return 1;
    }
    else
        if(printed[line][column] == 0 && flagged[line][column] == 0)
            if(mat.values[line][column] == -1)
                return -1;
                else
                    if(mat.values[line][column] != 0)
                    {
                        printed[line][column] = 1;
                        blocksToWin--;
                        return 1;
                    }
                    else
                    {
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

void run_game()
{
    int ok, result, blocksToWin;
    ok = 1;
    generate_matrix(bombs);
    blocksToWin = mat.n * mat.m - bombs;
    system("cls");

    while(ok && blocksToWin != 0)
    {
        print_matrix();
        result = input_move(blocksToWin);
        system("cls");
        if(result == -1)
        {
            cout<<"Ai pierdut!";
            ok = 0;
        }
    }
    if(blocksToWin == 0)
    {
        cout<<"YOU ARE A WINNER\n";
        print_matrix();
    }
}


void in_game_menu()
{
    system("cls");
    int k, option;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    cout<<"\n\tMINESWEEPER \n";

    k = 2;
    SetConsoleTextAttribute(hConsole, k);
    cout<<"\n\t1. USOR \n";

    k = 14;
    SetConsoleTextAttribute(hConsole, k);
    cout<<"\n\t2. MEDIU \n";

    k = 12;
    SetConsoleTextAttribute(hConsole, k);
    cout<<"\n\t3. EXPERT \n";

    k = 13;
    SetConsoleTextAttribute(hConsole, k);
    cout<<"\n\t4. CUSTOM \n";

    k = 15;
    SetConsoleTextAttribute(hConsole, k);
    cout<<"\n\t5. EXIT \n";

    cout<<"\nSelecteaza optiunea: ";
    cin>>option;

    switch(option)
    {
        case 1:
            mat.n = mat.m = 9;
            bombs = 10;
            run_game();
        break;

        case 2:
            mat.n = mat.m = 16;
            bombs = 40;
            run_game();
        break;

        case 3:
            mat.n = 30;
            mat.m = 16;
            bombs = 99;
            run_game();
        break;

        case 4:
            system("cls");
            cout<<"Introduceti inaltimea mapei: ";
            cin>>mat.n;
            if(mat.n < 9) mat.n = 9;
            if(mat.n > 24) mat.n = 24;

            cout<<"Introduceti latimea mapei: ";
            cin>>mat.m;
            if(mat.m < 9) mat.m = 9;
            if(mat.m > 30) mat.m = 30;

            cout<<"Introduceti numarul de bombe: ";
            cin>>bombs;
            if(bombs < 1) bombs = 1;
            if(bombs > (mat.n - 1) * (mat.m-1))
                bombs = (mat.n - 1) * (mat.m - 1);
            run_game();
            break;

        case 5:
            break;

        default:
            cout<<"Optiune incorecta! \n";
            system("pause");
            in_game_menu();
    }
}
