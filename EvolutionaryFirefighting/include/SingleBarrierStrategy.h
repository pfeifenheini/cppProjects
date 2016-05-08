#ifndef SINGLEBARRIERSTRATEGY_H
#define SINGLEBARRIERSTRATEGY_H

#include <vector>
#include <queue>
#include <random>

#include <time.h>
#include <math.h>

#include <mingw.thread.h>

#include <Direction.h>
#include <GridWorld.h>


struct Gene
{
    bool extendFront;
    Direction direction;
};

class SingleBarrierStrategy
{
    public:
        SingleBarrierStrategy(
                              float maxMutationProb,
                              int simulationSteps,
                              float income
                              );
        SingleBarrierStrategy(SingleBarrierStrategy *parent);
        SingleBarrierStrategy(
                              SingleBarrierStrategy *father,
                              SingleBarrierStrategy *mother
                              );
        virtual ~SingleBarrierStrategy();

        void copyStrategy(SingleBarrierStrategy *toCopy);

        void simulate(bool printSteps);
        void mutate();
        void increaseMutationProb(long maxMutationProb);
        inline bool enclosesFire() {return _enclosesFire;}
        inline bool isBlocked() {return _barrierBlocked;}

        inline int getFitness() {return _fitness;}
        inline int stepsNeeded() {return _stepsNeeded;}

        void printFinal();
        void printSteps();

    protected:

    private:
        int _simulationSteps;
        int _stepsNeeded;
        GridWorld *_grid;
        std::vector<Gene> *_strategyDNA;
        int _fitness;
        float _mutationProb;
        int _startX;
        int _startY;
        float _income;
        bool _barrierBlocked = false;
        bool _enclosesFire;
        default_random_engine _generator;
};

#endif // SINGLEBARRIERSTRATEGY_H
