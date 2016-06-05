#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include <time.h>

using namespace std;

int N = 1000;
int Z = 10000;

struct Point
{
    vector<double> *coordinates;
};

double distance(Point a, Point b)
{
    return 0;
}

int main()
{
    default_random_engine gen;
    gen.seed(time(0));
    uniform_real_distribution<double> rand(0,1);

    for(int i=0;i<10;i++)
    {
        cout << rand(gen) << endl;
    }

    vector<Point> *points = new vector<Point>(Z);
    vector<double> *coordinates;
    for(auto it = points->begin();it != points->end();it++)
    {
        coordinates = (*it).coordinates;
        for(int i=0;i<N;i++)
        {
            coordinates->push_back(rand(gen));
        }
    }

    cout << points->size() << endl;

    return 0;
}
