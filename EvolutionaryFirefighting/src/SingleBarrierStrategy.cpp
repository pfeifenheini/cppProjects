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
//        (*it).extendFront = true;
//        (*it).direction = EAST;
    }

    _startX = simulationSteps-1;
    _startY = simulationSteps;

    _simulationSteps = simulationSteps;
    _income = income;
//    _mutationProb = randMutationProb(_generator);
    _mutationProb = maxMutationProb;
    simulate(false);
}

SingleBarrierStrategy::SingleBarrierStrategy(SingleBarrierStrategy *parent)
{
    if(parent == nullptr) cout << "fuck";
    _startX = parent->_startX;
    _startY = parent->_startY;
    _simulationSteps = parent->_simulationSteps;
    _income = parent->_income;
    _barrierBlocked = parent->_barrierBlocked;
    _mutationProb = parent->_mutationProb;
    _grid = new GridWorld(2*_simulationSteps+1);
    _strategyDNA = new vector<Gene>(parent->_strategyDNA->begin(),parent->_strategyDNA->end());
    simulate(false);
}


SingleBarrierStrategy::SingleBarrierStrategy(SingleBarrierStrategy *father, SingleBarrierStrategy *mother)
{

}

SingleBarrierStrategy::~SingleBarrierStrategy()
{
    delete _strategyDNA;
    delete _grid;
}

void SingleBarrierStrategy::copyStrategy(SingleBarrierStrategy* toCopy)
{
    if(toCopy == nullptr) cout << "fuck";
    _startX = toCopy->_startX;
    _startY = toCopy->_startY;
    _simulationSteps = toCopy->_simulationSteps;
    _income = toCopy->_income;
    _barrierBlocked = toCopy->_barrierBlocked;
    _mutationProb = toCopy->_mutationProb;
    *_strategyDNA = *(toCopy->_strategyDNA);
    simulate(false);
}

void SingleBarrierStrategy::simulate(bool printSteps)
{
    _grid->reset();
    _grid->igniteCenter();
    _grid->protect(_startX, _startY);
    float budget = 1.0;
    auto geneIT = _strategyDNA->begin();
    Gene currentGene;
    Direction currentDir;
    Cell *curr;
    int x,y, stepsDone = 1;
    _stepsNeeded = 0;
    Cell barrierFront;
    Cell barrierBack;
    barrierFront.x = _startX;
    barrierFront.y = _startY;
    barrierBack.x = _startX;
    barrierBack.y = _startY;
    _enclosesFire = false;

    for(int step=0; step < _simulationSteps; step++)
    {
        int burningNow = _grid->getBurningCells();

        if(printSteps)
        {
            cout << "-----------" << endl;
            cout << "step: [" << step << "/" << _simulationSteps << "], " << stepsDone << " new cells protected" << endl;
            _grid->printGrid();
            Sleep(300);
            stepsDone = 0;
        }
        _stepsNeeded = step;

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
            _barrierBlocked = true;
            for(int tries=0; tries < 8; tries++)
            {
                x = curr->x+currentDir.horizontalDirection();
                y = curr->y+currentDir.verticalDirection();
                if(_grid->protect(x,y) == 1)
                {
                    curr->x = x;
                    curr->y = y;
                    budget -= 1;
                    stepsDone++;
                    if(!_barrierBlocked)
                        _barrierBlocked = false;
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

        _grid->spreadFire(step+1);

        if(_grid->getBurningCells() == burningNow)
        {
            if(printSteps)
            {
                cout << "-----------" << endl;
                cout << "step: [" << step+1 << "/" << _simulationSteps << "], " << stepsDone << " new cells protected" << endl;
                _grid->printGrid();
                Sleep(300);
                stepsDone = 0;
            }
            _stepsNeeded = step+1;
            _enclosesFire = true;
            break;
        }
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

void SingleBarrierStrategy::printFinal()
{
    _grid->printGrid();
    int steps = _stepsNeeded;
    if(!_enclosesFire) steps++;
    cout << steps << " steps needed";
    if(_enclosesFire) cout << " [enclosed] ";
    cout << endl;
}


void SingleBarrierStrategy::printSteps()
{
    simulate(true);
}

