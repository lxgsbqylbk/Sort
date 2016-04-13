#include<unistd.h>
#include<cstdio>

int main()
{
    FILE *fp = fopen("input.txt","rt+");
    if(!fp)
    {
        printf("fp is NULL\n");
        return -1;
    }
    int value=0;
    fscanf(fp,"%d",&value);
    printf("value is %d\n",value);
    if(0!=fseek(fp,0,SEEK_SET))
    {
        printf("fseek error\n");
        return -1;
    }
    truncate("input.txt",2);
    //fprintf(fp,"888999999\n");
    return 0;
}
