#include <iostream>
#include <vector>
#include <queue>
#include <random>

#include <conio.h>
#include <time.h>

#include <mingw.thread.h>
#include <mingw.mutex.h>

#include <SingleBarrierStrategy.h>

using namespace std;

const int populationSize = 10;
const int simulationSteps = 25;
const int generations = 10;
const float maxMutationProbability = 0.01;
const float income = 1.99999999;

void initialization()
{

}

void evaluation()
{

}

void selection()
{

}

void recombination()
{

}

void mutation()
{

}


int main()
{
    SingleBarrierStrategy *bestSoFar = new SingleBarrierStrategy(maxMutationProbability,simulationSteps,income);
    SingleBarrierStrategy *next = new SingleBarrierStrategy(bestSoFar);
    bestSoFar->print();
    next->print();
    int tries = 0;
    while(true)
    {
        if(kbhit())
        {
            char in = getch();
            if(in == 'x') break;
            if(in == 'r')
            {
                delete bestSoFar;
                delete next;
                bestSoFar = new SingleBarrierStrategy(maxMutationProbability,simulationSteps,income);
                next = new SingleBarrierStrategy(bestSoFar);
            }
        }
//        cout << next->getFitness() << " vs. " << bestSoFar->getFitness() << endl;
//        Sleep(100);
        if(next->getFitness() < bestSoFar->getFitness())
        {
            delete bestSoFar;
            bestSoFar = new SingleBarrierStrategy(next);
            bestSoFar->print();
            tries = 0;

            Sleep(1000);
        }
        if(tries > 100)
        {
            delete next;
            next = new SingleBarrierStrategy(bestSoFar);
        }

        next->mutate();
        next->simulate();
    }
}
