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
const int simulationSteps = 25;
const int generations = 10;
const float maxMutationProbability = 0.005;
const float income = 1.85; //enclosed at 1.67 after 52 steps
const int allowedMutations[5] = {1,5,10,100};
const int resetTime = 10;

mutex bestSoFar_mu, keepRunning_mu, timerRunnin_mu, allTimeBest_mu, resetTimer_mu;
SingleBarrierStrategy *bestSoFar = nullptr;
SingleBarrierStrategy *allTimeBest = nullptr;

bool keepRunning = true;
bool timerRunning = false;
bool resetTimer = false;

void simulate(int freedom, int threadID)
{
    bestSoFar_mu.lock();
    SingleBarrierStrategy *next = new SingleBarrierStrategy(bestSoFar);
    bestSoFar_mu.unlock();

    int mutationCounter = 0;
    keepRunning_mu.lock();
    bool running = keepRunning;
    keepRunning_mu.unlock();
    while(running)
    {
//        cout << next->getFitness() << " vs. " << bestSoFar->getFitness() << endl;
//        Sleep(100);
        bestSoFar_mu.lock();
        if(next->getFitness() < bestSoFar->getFitness())
        {
            cout << "current best: " << next->getFitness() << " cells on fire!";
//            cout << " Found after " << setw(2) << mutationCounter << " mutations,";
//            cout << " by thread " << threadID;
//            cout << " [";
//            if(next->isBlocked()) cout << "un";
//            cout << "blocked]";
            bestSoFar->copyStrategy(next);
            if(bestSoFar->enclosesFire())
                cout << " --> Fire enclosed after " << bestSoFar->stepsNeeded() << "/" << simulationSteps << " steps <--";
            cout << endl;
//            bestSoFar->printFinal();
            mutationCounter = 0;
//            Sleep(500);
            allTimeBest_mu.lock();
            if(bestSoFar->getFitness() < allTimeBest->getFitness())
            {
                allTimeBest->copyStrategy(bestSoFar);
            }
            allTimeBest_mu.unlock();
            resetTimer_mu.lock();
            resetTimer = true;
            resetTimer_mu.unlock();
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

        keepRunning_mu.lock();
        running = keepRunning;
        keepRunning_mu.unlock();
    }

    delete next;
}

void stopThreads(vector<thread*> *threads)
{
    keepRunning_mu.lock();
    keepRunning = false;
    keepRunning_mu.unlock();

    for(auto it = threads->begin();it != threads->end();it++)
    {
        (*it)->join();
    }
}

void reset(vector<thread*> *threads)
{
    stopThreads(threads);

    keepRunning = true;

    delete bestSoFar;
    bestSoFar = new SingleBarrierStrategy(maxMutationProbability,simulationSteps,income);

    int threadID = 1;
    for(auto it = threads->begin();it != threads->end();it++)
    {
        (*it) = new thread(simulate,allowedMutations[threadID-1],threadID);
        threadID++;
    }
}

void startTimer(vector<thread*> *threads, int seconds)
{
    cout << "-- start timer --" << endl;
    timerRunnin_mu.lock();
    bool running = timerRunning;
    timerRunnin_mu.unlock();
//    bool enclosingFound = false;
    while(running)
    {
        for(int i=0;i<seconds;i++)
        {
            timerRunnin_mu.lock();
            running = timerRunning;
            timerRunnin_mu.unlock();

            resetTimer_mu.lock();
            if(resetTimer)
            {
                resetTimer = false;
                i=0;
            }
            resetTimer_mu.unlock();

            if(running == false) return;
            if(seconds-i <= 3)
                cout << "-- reset in " << seconds-i << " --" << endl;
            Sleep((DWORD)(1000));
        }

//        bestSoFar_mu.lock();
//        enclosingFound = bestSoFar->enclosesFire();
//        bestSoFar_mu.unlock();
//        if(enclosingFound) break;

        cout << "-- reset (timer) --" << endl;
        reset(threads);
    }
}

void stopTimer(thread* timer)
{
    timerRunnin_mu.lock();
    timerRunning = false;
    timerRunnin_mu.unlock();

    timer->join();
    delete timer;
    cout << "-- timer stopped --" << endl;
}

int main()
{
    bestSoFar = new SingleBarrierStrategy(maxMutationProbability,simulationSteps,income);
    allTimeBest = new SingleBarrierStrategy(bestSoFar);

    thread *t1 = new thread(simulate,allowedMutations[0],1);
    thread *t2 = new thread(simulate,allowedMutations[1],2);
    thread *t3 = new thread(simulate,allowedMutations[2],3);
    thread *t4 = new thread(simulate,allowedMutations[3],4);
    vector<thread*> *threads = new vector<thread*>(4);
    threads->at(0) = t1;
    threads->at(1) = t2;
    threads->at(2) = t3;
    threads->at(3) = t4;

    thread *timer = nullptr;

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
            cout << "-- best so far --" << endl;
            bestSoFar->printFinal();
        }

        allTimeBest_mu.lock();
        if(in == 'w')
        {
            cout << "-- all time best --" << endl;
            allTimeBest->printFinal();
        }

        if(in == 'q')
        {
            cout << "-- start animation" << endl;
            allTimeBest->printSteps();
            cout << "-- continue search --" << endl;
        }
        allTimeBest_mu.unlock();
        bestSoFar_mu.unlock();

        if(in == 'r')
        {
            cout << "-- reset --" << endl;

            resetTimer_mu.lock();
            resetTimer = true;
            resetTimer_mu.unlock();

            reset(threads);
        }

        if(in == 't')
        {
            if(timerRunning)
            {
                stopTimer(timer);
            }
            else
            {
                timerRunning = true;
                timer = new thread(startTimer,threads,resetTime);
            }
        }

        in = getch();
    }

    cout << "-- all time best --" << endl;
    allTimeBest->printFinal();
    cout << "income = " << income << ", simulation steps = " << simulationSteps << ", timer = " << resetTime << "s" << endl;

    stopThreads(threads);
}
