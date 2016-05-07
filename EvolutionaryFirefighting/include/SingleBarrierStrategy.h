#ifndef SINGLEBARRIERSTRATEGY_H
#define SINGLEBARRIERSTRATEGY_H

#include <vector>
#include <queue>
#include <random>

#include <time.h>
#include <math.h>

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

        void simulate();
        void mutate();
        void increaseMutationProb(long maxMutationProb);

        inline int getFitness() {return _fitness;}

        void print();

    protected:

    private:
        int _simulationSteps;
        GridWorld *_grid;
        std::vector<Gene> *_strategyDNA;
        int _fitness;
        float _mutationProb;
        int _startX;
        int _startY;
        float _income;
        default_random_engine _generator;
};

#endif // SINGLEBARRIERSTRATEGY_H
