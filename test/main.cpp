#include <iostream>
#include <limits>
#include <queue>
#include <math.h>

#include <conio.h>
#include <mingw.thread.h>
#include <mingw.mutex.h>

using namespace std;

int currentNumber = 3;
int primeNumbersPrinted = 0;
int largestPrime = 2;

bool keepWorking = true;

mutex mu_currentNumber, mu_print, mu_keepWorking;

bool isPrime(int n)
{
    if(n <= 1) return false;

    if(n <= 3) return true;

    if(n%2 == 0 || n%3 == 0) return false;

    for(int i=5;i*i <= n;i+=6)
    {
        if(n%i == 0 || n%(i+2) == 0) return false;
    }
    return true;
}

void calcPrimes(int id)
{
    bool working = true;

    while(working)
    {
        mu_currentNumber.lock();
        int toCheck = currentNumber;
        currentNumber+=2;
        mu_currentNumber.unlock();

        if(isPrime(toCheck))
        {
            mu_print.lock();
            //cout << toCheck << endl;
            primeNumbersPrinted++;
            if(toCheck > largestPrime)
                largestPrime = toCheck;
            mu_print.unlock();
        }

        mu_keepWorking.lock();
        if(!keepWorking)
            working = false;
        mu_keepWorking.unlock();
    }
}

int main()
{
    thread t1(calcPrimes,1);
    thread t2(calcPrimes,2);
    thread t3(calcPrimes,3);
    thread t4(calcPrimes,4);

    while(true)
    {
        Sleep(1000);
        mu_print.lock();
        cout << largestPrime << endl;
        if(kbhit())
        {
            getch();
            keepWorking = false;
            mu_print.unlock();
            break;
        }
        mu_print.unlock();
    }

    mu_keepWorking.lock();
    keepWorking = false;
    mu_keepWorking.unlock();

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
