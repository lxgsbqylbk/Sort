#include<cstdio>
#include<vector>
#include<algorithm>

using namespace std;


vector<int> vec(10000000);

int main(int argc,char *argv[])
{
    vec.clear();
    int value=0;

    while(~scanf("%d",&value))
    {
        vec.push_back(value);
    }
    sort(vec.begin(),vec.end());
    //reverse(vec.begin(),vec.end());
    for(size_t i=0;i<vec.size();i++)
    {
        printf("%d\n",vec[i]);
    }
    //printf("vector size %d\n",(int)vec.size());
    return 0;
}
