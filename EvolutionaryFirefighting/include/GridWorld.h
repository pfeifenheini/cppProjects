#ifndef GRIDWORLD_H
#define GRIDWORLD_H

#include <iostream>
#include <vector>
#include <queue>

#define EMPTY 0
#define PROTECTED 1
#define BURNING 2

using namespace std;

struct Cell
{
    int x;
    int y;
    int state;
    int fireTime;
};

class GridWorld
{
    public:
        GridWorld(int dimension);
        virtual ~GridWorld();

        bool isOnGrid(int x, int y);
        int ignite(int x, int y, int time);
        int igniteCenter();
        int protect(int x, int y);
        void spreadFire(int time);
        void reset();
        void printGrid();

        inline int getBurningCells() {return _burningCellsTotal;}
        inline int getDimension() {return _dimension;}

    protected:

    private:
        vector<vector<Cell>* > *_grid = nullptr;
        int _dimension;
        queue<Cell> *_burningEdgeCells = nullptr;
        int _burningCellsEdge = 0;
        int _burningCellsTotal = 0;
};

#endif // GRIDWORLD_H
