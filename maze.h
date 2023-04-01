#ifndef maze_h
#define maze_h

#include <iostream>
#include <vector>

using namespace std;

struct Cell {
public:
    bool up,down,left,right,visited;
    int rows,cols;
    Cell(int x_coor,int y_coor)
    {
        rows = y_coor;
        cols = x_coor;
        up = true;
        down = true;
        left = true;
        right = true;
        visited = false;
    }
};

class maze
{
private:
    int row, col;
    int mazeID;
    vector<vector<Cell>> mazeMap;
public:
    vector<vector<Cell>> MazeGenerator(int M, int N, int mazeID);
    void PathFinder(int Xa, int Ya, int Xb, int Yb, int M, int N);
    maze(int row, int col, int mazeID);
    int Random(unsigned long size);
};

#endif /* maze_h */
