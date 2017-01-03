#include <cstdlib>
#include <ctime>
#include <iostream>
#include "declarations.h"
#include <cstring>

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

int input_move()
{
    Queue C[10201], v, z;
    int p, u, i;
    cin>>inputMove;

    if(inputMove[0] == 'F')
    {
        flagged[inputMove[1] - 48][inputMove[2] - 48] = 1;
        return 1;
    }
    else
        if(mat.values[inputMove[0] - 48][inputMove[1] - 48] == -1)
            return -1;
            else
                if(mat.values[inputMove[0] - 48][inputMove[1] - 48] != 0)
                {
                    printed[inputMove[0] - 48][inputMove[1] - 48] = 1;
                    return 1;
                }
                else
                {
                    p = u = 1;
                    C[p].row = inputMove[0] - 48;
                    C[p].column = inputMove[1] - 48;

                    while(p <= u)
                    {
                        v = C[p];
                        p++;
                        printed[v.row][v.column] = 1;
                        for(i = 0; i < 8; i++)
                        {
                            z.row = v.row + dx[i];
                            z.column = v.column + dy[i];

                            if(z.row >= 1 && z.row <= mat.n && z.column >= 1 && z.column <= mat.m && printed[z.row][z.column] == 0)
                                if(mat.values[z.row][z.column] != 0)
                                printed[z.row][z.column] = 1;
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

void print_matrix()
{
    int i, j;
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
                        if(mat.values[i][j] == -1)
                            cout<<'B';
                        else
                            cout<<(int)mat.values[i][j]<<" ";
                else
                    cout<<'+'<<" ";
        cout<<'\n';
    }
}
