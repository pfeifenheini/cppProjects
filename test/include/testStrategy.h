#ifndef TESTSTRATEGY_H
#define TESTSTRATEGY_H

#include <iostream>

using namespace std;

class testStrategy
{
    public:
        testStrategy();
        virtual ~testStrategy();

        void printMessage();
    protected:
    private:
};

#endif // TESTSTRATEGY_H
