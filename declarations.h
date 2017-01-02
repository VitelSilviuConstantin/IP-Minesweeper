struct matrix
{
    int n;
    int m;
    char values[101][101];
}mat;

bool printed[101][101];

int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};
