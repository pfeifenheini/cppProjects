#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>

/**
 * Authors:
 * Roland Meneghetti
 * Martin Kretschmer
 *
 * Artificial Life Lecture
 * Summer Term 2016
 *
 * Exercises sheet 7
 * Programming Assignment: D
 * Evolutionary Algorithm
 *
 * compile with:
 * g++ -std=c++11 main.cpp -o main
 *
 * run with 'java CellularAutomaton'
 *
 * Usage:
 * Choose the initial pattern in the dialogue box.
 * Possible choices:
 *  - Blinker
 *  - Glider
 *  - r-Pentamino
 *  - Glider Gun
 *  - Random
 *
 * Click 'OK' to start the simulation.
 *
 * A file named 'LogFile.txt' is created, containing per
 * line the number of living cells at each time.
 *
 * Problems:
 * It is possible that the scene will not show every
 * intermediate step. That behavior was observed under
 * a Linus OS, but not Windows.
 * Is this the case, run the program as
 *
 *  'java CellularAutomaton slow'
 *
 * to increase the time delay between the animation
 * steps
 *
 * Exit:
 * Just close the window.
 *
 */

#define P 2 //population size
#define L 2 //genome size
#define EPSILON 0.01 //parameter for mutation

using namespace std;

default_random_engine gen;

struct Individual
{
    double fitness;
    double genome[L];
};

//I found a different definition on the web, where the last part is exp(-y) instead of exp(-x)
double f_hosaki(double x, double y)
{
    return (1.0+x*(-8.0+x*(7.0+x*((-7.0/3.0)+x*(1.0/4.0)))))*y*y*exp(-y);
}

//initialization with random values between 0 and 5
void initialization(Individual *population)
{
    uniform_real_distribution<double> initialGenome(0,5); //generates random values between 0 and 5
    for(int i=0;i<P;i++)
    {
        for(int j=0;j<L;j++)
        {
            population[i].genome[j] = initialGenome(gen);
        }
    }
}

void fitness_evaluation(Individual *population)
{
    //calculate fitness for each individual
    for(int i=0;i<P;i++)
    {
        population[i].fitness = f_hosaki(population[i].genome[0],population[i].genome[1]);
    }
}

// swaps the best individual to the first position
// returns true if swap was necessary
bool external_selection(Individual *population)
{
    bool best_has_changed = false;

    //put best individual at first position of the array
    for(int i=1;i<P;i++)
    {
        if(population[i].fitness < population[0].fitness)
        {
            //swap
            Individual tmp = population[0];
            population[0] = population[i];
            population[i] = tmp;
            best_has_changed = true;
        }
    }
    return best_has_changed;
}

// not implemented
void parent_selection(Individual *population)
{

}

void inheritance(Individual *population)
{
    population[1] = population[0]; // first/best individual is copied
}

// Random vector is added to second individual
void mutation(Individual *population)
{
    uniform_real_distribution<double> r(-EPSILON,EPSILON); // generates random values between -EPSILON and EPSILON
    for(int i=0;i<L;i++)
    {
        population[1].genome[i] += r(gen);
    }
}


// prints each individual to the standard output
// Each individual is represented by "identifier = {fitness | genome}"
void printStatus(Individual *population)
{
    for(int i=0;i<P;i++)
    {
        cout << "Ind" << i << " = {" << population[i].fitness << " | ";
        for(int j=0;j<L;j++)
        {
            cout << population[i].genome[j] << ", ";
        }
        cout << "\b\b}" << endl;
    }
    cout << endl;
}

int main()
{
    gen.seed(time(0)); //generate different numbers with each run of the program

    Individual population[P];

    int unsuccessfull_generation = 0; // counter for "unsuccessful" mutations
    bool improved; // indicator to see if a generation has improved compared to the previous

    initialization(population);
    fitness_evaluation(population);
    external_selection(population);

    // calculation stops, if 100 consecutive generations don't yield an improvement
    while(unsuccessfull_generation < 100)
    {
        //printStatus(population);
        cout << "fittnes:" << population[0].fitness << ", " << population[1].fitness << endl;
        parent_selection(population);
        inheritance(population);
        mutation(population);

        fitness_evaluation(population);
        improved = external_selection(population);

        if(improved)
            unsuccessfull_generation = 0;
        else
            unsuccessfull_generation++;


    }
    cout << "fittnes: ind0 = " << population[0].fitness << ", ind1 = " << population[1].fitness << endl;
    printStatus(population);

    return 0;
}
