#ifndef SINGLEBARRIERSTRATEGY_H
#define SINGLEBARRIERSTRATEGY_H

#include <vector>
#include <queue>
#include <math.h>

#include <Direction.h>

#define EMPTY 0
#define PROTECTED 1
#define BURNING 2

struct Gene
{
    bool extendFront;
    Direction direction;
};

struct Coords
{
    int x;
    int y;
};

class SingleBarrierStrategy
{
    public:
        SingleBarrierStrategy(long maxMutationProb, int simulationSteps);
        SingleBarrierStrategy(SingleBarrierStrategy father, SingleBarrierStrategy mother);
        virtual ~SingleBarrierStrategy();

        void evaluate(int simulationSteps);
        void mutate();
        void increaseMutationProb(long maxMutationProb);

        inline int getFitness() {return fitness;}

    protected:

    private:
        std::vector<Gene> *strategyDNA;
        int fitness;
        long mutationProb;
        int startX;
        int startY;
        long income 2.0;
        Coords barrierFront;
        Coords barrierBack;
        queue<Coords> burningEdgeCells;
};

#endif // SINGLEBARRIERSTRATEGY_H
