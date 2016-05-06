#include "SingleBarrierStrategy.h"

SingleBarrierStrategy::SingleBarrierStrategy(long maxMutationProb)
{

}

SingleBarrierStrategy::SingleBarrierStrategy(SingleBarrierStrategy father, SingleBarrierStrategy mother)
{

}

SingleBarrierStrategy::~SingleBarrierStrategy()
{
    //dtor
}

void SingleBarrierStrategy::evaluate(int simulationSteps)
{
    using namespace std;

    vector<int> column(2*simulationSteps+1,EMPTY);
    vector<vector<int> > grid(2*simulationSteps+1,column);

    long budget = 1.0;

    int currX = startX;
    int currY = startY;
    int dnaPos = 0;

    grid[simulationSteps+1][simulationSteps+1] = BURNING;
    grid[currX][currY] = PROTECTED;

    for(int i=0;i<simulationSteps;i++)
    {
        Direction currDir = strategyDNA[dnaPos];

        currX += currDir.horizontalDirection();
        currY += currDir.verticalDirection();

        if(currX < 0) currX = 0;
        else if(currX > 2*simulationSteps) currX = 2*simulationSteps;

        if(currY < 0) currX = 0;
        else if(currY > 2*simulationSteps) currY = 2*simulationSteps;

        if(grid[currX][currY] == EMPTY)
        {
            grid[currX][currY] = PROTECTED;
            dnaPos++;
            budget -= 1;
        }

    }

}

void SingleBarrierStrategy::mutate()
{

}

void SingleBarrierStrategy::increaseMutationProb(long maxMutationProb)
{

}
