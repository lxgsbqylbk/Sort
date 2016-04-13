#include<time.h>
#include<stdio.h>
#include<stdlib.h>

const int N = 1000000000;
const int UPPERBOUND = 500000000;

inline int getRand()
{
    return (rand()<<15)+rand();
}
int main()
{
    srand(int(time(NULL)));
    for(int i=0;i<N;i++)
    {
        int value = getRand()%UPPERBOUND;
        if(rand()%2) value=-value;
        printf("%d\n",value);
    }
    return 0;
}
