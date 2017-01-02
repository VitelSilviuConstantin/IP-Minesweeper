#include <cstdlib>
#include <ctime>
#include <iostream>
#include "declarations.h"


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


void print_matrix()
{
    int i, j;
    for(i = 1; i <= mat.n; i++)
    {
        for(j = 1; j <= mat.m; j++)
            if(mat.values[i][j] == -1)
                cout<<'B'<<" ";
            else
                cout<<(int)mat.values[i][j]<<" ";
        cout<<'\n';
    }
}
