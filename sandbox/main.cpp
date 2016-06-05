#include <iostream>
#include <cmath>

using namespace std;

void fibonacci(int* A, int* B)
{
    int tmp = *B;
    *B = *B + *A;
    *A = tmp;
}

int main()
{

    return 0;
}
