#include "spliter.h"

spliter s;
int main(int argc,char* argv[])
{
    timeval start_main,end_main;
    if(argc!=3)
    {
        printf("argv error\n");
        return 0;
    }

    gettimeofday(&start_main,NULL);

    if(s.splitFile(argv[1])!=0)
    {
        return -1;
    }
    //if(s.doSort()!=0)
    //{
    //    return -1;
    //}
    if(s.doMultipleSort()!=0)
    {
        return -1;
    }
    if(s.doMerge(argv[2])!=0)
    {
        return -1;
    }

    gettimeofday(&end_main,NULL);

    printf("main time : %.9fs\n",(end_main.tv_sec-start_main.tv_sec) + (end_main.tv_usec-start_main.tv_usec)/1000000.0);

    printf("MAIN STOP\n");
    return 0;
}
