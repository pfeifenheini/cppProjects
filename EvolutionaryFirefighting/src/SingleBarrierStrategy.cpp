#include "SingleBarrierStrategy.h"

SingleBarrierStrategy::SingleBarrierStrategy(
    float maxMutationProb,
    int simulationSteps,
    float income
)
{
    _generator.seed(time(0));
    uniform_real_distribution<float> randMutationProb(0,maxMutationProb);
    uniform_int_distribution<int> randDirection(-1,7);
    uniform_int_distribution<int> randFrontEnd(0,1);


    _grid = new GridWorld(2*simulationSteps+1);
    _strategyDNA = new vector<Gene>(2*simulationSteps);

    for(auto it = _strategyDNA->begin(); it != _strategyDNA->end(); it++)
    {
        (*it).extendFront = (bool)randFrontEnd(_generator);
        (*it).direction = randDirection(_generator);
    }

    _startX = simulationSteps-1;
    _startY = simulationSteps-1;

    _simulationSteps = simulationSteps;
    _income = income;
//    _mutationProb = randMutationProb(_generator);
    _mutationProb = maxMutationProb;
    simulate();
}

SingleBarrierStrategy::SingleBarrierStrategy(SingleBarrierStrategy *parent)
{
    if(parent == nullptr) cout << "fuck";
    _startX = parent->_startX;
    _startY = parent->_startY;
    _simulationSteps = parent->_simulationSteps;
    _income = parent->_income;
    _mutationProb = parent->_mutationProb;
    _grid = new GridWorld(2*_simulationSteps+1);
    _strategyDNA = new vector<Gene>(parent->_strategyDNA->begin(),parent->_strategyDNA->end());
    simulate();
}


SingleBarrierStrategy::SingleBarrierStrategy(SingleBarrierStrategy *father, SingleBarrierStrategy *mother)
{

}

SingleBarrierStrategy::~SingleBarrierStrategy()
{
    delete _strategyDNA;
    delete _grid;
}

void SingleBarrierStrategy::simulate()
{
    _grid->reset();
    _grid->igniteCenter();
    _grid->protect(_startX, _startY);
    float budget = 1.0;
    auto geneIT = _strategyDNA->begin();
    Gene currentGene;
    Direction currentDir;
    Cell *curr;
    int x,y;
    Cell barrierFront;
    Cell barrierBack;
    barrierFront.x = _startX;
    barrierFront.y = _startY;
    barrierBack.x = _startX;
    barrierBack.y = _startY;

    for(int step=0; step < _simulationSteps; step++)
    {
//        cout << "sim step: " << step << endl;
//        _grid->printGrid();

        while(budget >= 1 && geneIT != _strategyDNA->end())
        {
            currentGene = (*geneIT);
            if(currentGene.extendFront)
            {
                curr = &barrierFront;
            }
            else
            {
                curr = &barrierBack;
            }
            currentDir = currentGene.direction;
            for(int tries=0; tries < 8; tries++)
            {
                x = curr->x+currentDir.horizontalDirection();
                y = curr->y+currentDir.verticalDirection();
                if(_grid->protect(x,y) == 1)
                {
                    curr->x = x;
                    curr->y = y;
                    budget -= 1;
                    break;
                }
                if(currentGene.extendFront)
                {
                    currentDir.nextCW();
                }
                else
                {
                    currentDir.nextCCW();
                }
            }
            geneIT++;

        }
        budget+=_income;

        _grid->spreadFire();
    }
    _fitness = _grid->getBurningCells();
}

void SingleBarrierStrategy::mutate()
{
    uniform_real_distribution<float> rand(0,1);
    uniform_int_distribution<int> newDirection(-1,7);
    for(auto it = _strategyDNA->begin();it != _strategyDNA->end();it++)
    {
        if(rand(_generator) < _mutationProb)
        {
            if(rand(_generator) <= 0.5)
            {
                (*it).extendFront = (bool)(1-(*it).extendFront);
            }
            (*it).direction = newDirection(_generator);
        }
    }

}

void SingleBarrierStrategy::increaseMutationProb(long maxMutationProb)
{

}

void SingleBarrierStrategy::print()
{
    _grid->printGrid();
}
