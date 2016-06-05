#include "GridWorld.h"

GridWorld::GridWorld(int dimension)
{
    vector<vector<Cell>* > *grid = new vector<vector<Cell>* >(dimension);
//    for(int i=0;i<dimension;i++)
    for(auto it = grid->begin();it != grid->end();it++)
    {
        *it = new vector<Cell>(dimension);
    }


    for(int i=0;i<dimension;i++)
    {
        for(int j=0;j<dimension;j++)
        {
            grid->at(i)->at(j).x = i;
            grid->at(i)->at(j).y = j;
            grid->at(i)->at(j).state = EMPTY;
        }
    }

    _grid = grid;
    _dimension = dimension;
    _burningEdgeCells = new queue<Cell>();
}

GridWorld::~GridWorld()
{
    for(int i=0;i<_dimension;i++)
    {
        delete _grid->at(i);
    }
    delete _burningEdgeCells;
    delete _grid;
}

inline bool GridWorld::isOnGrid(int x, int y)
{
    if(x < 0 || x >= _dimension) return false;
    if(y < 0 || y >= _dimension) return false;
    return true;
}

// returns 1 it cell has changed state and 0 else
int GridWorld::ignite(int x, int y, int time)
{
    if(isOnGrid(x,y))
        if(_grid->at(x)->at(y).state == EMPTY)
        {
            _grid->at(x)->at(y).state = BURNING;
            _grid->at(x)->at(y).fireTime = time;
            _burningEdgeCells->push(_grid->at(x)->at(y));
            _burningCellsEdge++;
            _burningCellsTotal++;
            return 1;
        }
    return 0;
}

int GridWorld::igniteCenter()
{
    return ignite((int)_dimension/2,(int)_dimension/2,0);
}


// returns 1 it cell has changed state and 0 else
int GridWorld::protect(int x, int y)
{
        if(isOnGrid(x,y))
        {
            if(_grid->at(x)->at(y).state == EMPTY)
            {
                _grid->at(x)->at(y).state = PROTECTED;
                return 1;
            }
//            if(_grid->at(x)->at(y).state == BURNING && x != (int)(_dimension/2) && y != (int)(_dimension/2))
//            if(_grid->at(x)->at(y).state == BURNING)
//            {
//                _grid->at(x)->at(y).state = PROTECTED;
//                _burningCellsTotal--;
//                return 1;
//            }
        }

    return 0;
}

void GridWorld::spreadFire(int time)
{
    Cell c;
    int currentEdgeSize = _burningCellsEdge;

    for(int i=0;i<currentEdgeSize;i++)
    {
        c = _burningEdgeCells->front();
        ignite(c.x+1,c.y,time);
        ignite(c.x,c.y+1,time);
        ignite(c.x-1,c.y,time);
        ignite(c.x,c.y-1,time);
        _burningEdgeCells->pop();
        _burningCellsEdge--;
    }
}


void GridWorld::reset()
{
    for(int i=0;i<_dimension;i++)
    {
        for(int j=0;j<_dimension;j++)
        {
            _grid->at(i)->at(j).state = EMPTY;
            _grid->at(i)->at(j).fireTime = 0;
        }
    }
    delete _burningEdgeCells;
    _burningEdgeCells = new queue<Cell>();
    _burningCellsEdge = 0;
    _burningCellsTotal = 0;
}

void GridWorld::printGrid()
{
    int minX=_dimension-1, minY=_dimension-1, maxX=0, maxY=0;
    int state, fireTime;

    for(int i=0;i<_dimension;i++)
    {
        for(int j=0;j<_dimension;j++)
        {
            state = _grid->at(j)->at(_dimension-1-i).state;
            if(state != EMPTY)
            {
                if(i<minX) minX = i;
                if(j<minY) minY = j;
                if(i>maxX) maxX = i;
                if(j>maxY) maxY = j;
            }
        }
    }

    for(int i=minX;i<=maxX;i++)
    {
        for(int j=minY;j<=maxY;j++)
        {
            state = _grid->at(j)->at(_dimension-1-i).state;
            fireTime = _grid->at(j)->at(_dimension-1-i).fireTime;

            if(i==(int)(_dimension/2) && j==(int)(_dimension/2))
            {
                cout << "S" << " ";
                continue;
            }

            switch(state)
            {
            case BURNING:
                cout << fireTime%10 << " ";
                break;
            case PROTECTED:
                cout << "X" << " ";
                break;
            default:
                cout << "  ";
            };
        }
        cout << endl;
    }
    cout << _burningCellsTotal << " burning cells" << endl;
}
