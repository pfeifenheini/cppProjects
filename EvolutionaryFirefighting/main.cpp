#include <iostream>
#include <iomanip>
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
const int simulationSteps = 100;
const int generations = 10;
const float maxMutationProbability = 0.005;
const float income = 1.75;

mutex bestSoFar_mu, running_mu, reset_mu;
SingleBarrierStrategy *bestSoFar = nullptr;

bool keepRunning = true;
bool reset = false;

void simulate(int freedom, int threadID)
{
    bestSoFar_mu.lock();
    SingleBarrierStrategy *next = new SingleBarrierStrategy(bestSoFar);
    bestSoFar_mu.unlock();

    int mutationCounter = 0;
    running_mu.lock();
    bool running = keepRunning;
    running_mu.unlock();
    while(running)
    {

//        cout << next->getFitness() << " vs. " << bestSoFar->getFitness() << endl;
//        Sleep(100);
        bestSoFar_mu.lock();
        if(next->getFitness() < bestSoFar->getFitness())
        {
            cout << "Best protects " << next->getFitness() << " cells!";
            cout << " Found after " << setw(4) << mutationCounter << " mutations";
            cout << " by thread " << threadID;
            if(bestSoFar->enclosesFire())
                cout << " --> Fire enclosed <--";
            cout << endl;
            bestSoFar->copyStrategy(next);
//            bestSoFar->printFinal();
            mutationCounter = 0;
//            Sleep(500);
        }
        if(freedom != 0)
        {
            if(mutationCounter > freedom && freedom != 0)
            {
                next->copyStrategy(bestSoFar);
                mutationCounter = 0;
            }
        }
        bestSoFar_mu.unlock();

        next->mutate();
        mutationCounter++;
        next->simulate(false);

        running_mu.lock();
        running = keepRunning;
        running_mu.unlock();
    }

    delete next;
}

int main()
{
    bestSoFar = new SingleBarrierStrategy(maxMutationProbability,simulationSteps,income);

    thread *t1 = new thread(simulate,1,1);
    thread *t2 = new thread(simulate,5,2);
    thread *t3 = new thread(simulate,10,3);
    thread *t4 = new thread(simulate,100,4);

    char in = getch();
    while(in != 'x')
    {
        bestSoFar_mu.lock();
        if(in == 'a')
        {
            cout << "-- start animation" << endl;
            bestSoFar->printSteps();
            cout << "-- continue search --" << endl;
        }
        if(in == 's')
        {
            cout << "best so far: " << endl;
            bestSoFar->printFinal();
        }
        bestSoFar_mu.unlock();

        if(in == 'r')
        {
            keepRunning = false;

            t1->join();
            t2->join();
            t3->join();
            t4->join();

            keepRunning = true;

            delete bestSoFar;
            bestSoFar = new SingleBarrierStrategy(maxMutationProbability,simulationSteps,income);

            t1 = new thread(simulate,1,1);
            t2 = new thread(simulate,5,2);
            t3 = new thread(simulate,10,3);
            t4 = new thread(simulate,100,4);
        }

        in = getch();
    }

    running_mu.lock();
    keepRunning = false;
    running_mu.unlock();

    t1->join();
    t2->join();
    t3->join();
    t4->join();
}
