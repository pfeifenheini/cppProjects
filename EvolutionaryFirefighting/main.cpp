#include <iostream>
#include <vector>
#include <queue>

#include <mingw.thread.h>
#include <mingw.mutex.h>

#include <SingleBarrierStrategy.h>

using namespace std;

const int populationSize = 10;
const int simulationSteps = 10;
const int generations = 10;
const long maxMutationProbability = 0.01;

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
    vector<int> a(5,0);
    vector<vector<int> > m(10,a);
    int b=1;

    for(unsigned int i=0;i<m.size();i++)
    {
        for(unsigned int j=0;j<m[i].size();j++)
        {
            m[i][j] = b++;
            cout << m[i][j] << " ";
        }
        cout << endl;
    }

}
