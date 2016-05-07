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
const int simulationSteps = 30;
const int generations = 10;
const float maxMutationProbability = 0.01;
const float income = 1.8;

mutex bestSoFar_mu, running_mu, reset_mu;
SingleBarrierStrategy *bestSoFar = nullptr;

bool keepRunning = true;
bool reset = false;

void simulate(int freedom, int threadID)
{
    bestSoFar_mu.lock();
    SingleBarrierStrategy *next = new SingleBarrierStrategy(bestSoFar);
    bestSoFar_mu.unlock();

    int tries = 0;
    bool running = keepRunning;
    while(running)
    {

//        cout << next->getFitness() << " vs. " << bestSoFar->getFitness() << endl;
//        Sleep(100);
        bestSoFar_mu.lock();
        if(next->getFitness() < bestSoFar->getFitness())
        {
            cout << "thread " << threadID << " succeeds" << endl;
            delete bestSoFar;
            bestSoFar = new SingleBarrierStrategy(next);
            bestSoFar->print();
            tries = 0;
            Sleep(500);
        }
        if(freedom != 0)
        {
            tries++;
            if(tries > freedom && freedom != 0)
            {
                next->copyStrategy(bestSoFar);
            }
        }
        bestSoFar_mu.unlock();

        next->mutate();
        next->simulate();

        running_mu.lock();
        running = keepRunning;
        running_mu.unlock();
    }
}

int main()
{
    bestSoFar = new SingleBarrierStrategy(maxMutationProbability,simulationSteps,income);

    thread t1(simulate,0,1);
    thread t2(simulate,10,2);
    thread t3(simulate,100,3);
    thread t4(simulate,1000,4);

    char in = getch();
    while(in != 'x')
    {
        in = getch();
        if(in == 'r')
        {
        }
    }

    running_mu.lock();
    keepRunning = false;
    running_mu.unlock();

    t1.join();
    t2.join();
    t3.join();
    t4.join();

}
