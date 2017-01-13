#include <windows.h>

struct matrix
{
    int n;
    int m;
    char values[101][101];
}mat;

struct Queue
{
   int row;
   int column;
};

struct position
{
    int row;
    int column;
    bool flag;
}input;

bool flagged[101][101];
bool printed[101][101];

int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};

int bombs;
bool bombFirst, printBombs;
int minesLeft;

COORD MouseWhere;

