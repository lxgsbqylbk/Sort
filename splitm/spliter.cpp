#include "spliter.h"

const char* spliter::TMPDIR = "./tmpdir";

int spliter::createDir(const char* dirPath)
{
    if(!dirPath) return -1;
    if(NULL==opendir(dirPath))
    {
        if(0!=mkdir(dirPath,S_IRWXU | S_IRWXG | S_IRWXO))
        {
            printf("create dir %s error\n",dirPath);
            return -1;
        }
    }
    return 0;
}

int spliter::splitFile(const char* filePath)
{
    if(!filePath)
    {
        printf("filepath is NULL\n");
        return -1;
    }
    if(!(ifp=fopen(filePath,"r")))
    {
        printf("open filepath failed\n");
        return -1;
    }
    if(0!=createDir(TMPDIR))
    {
        return -1;
    }
    char tmp[16];
    for(int i=0;i<FILENUM;i++)
    {
        strcpy(outPath[i],TMPDIR);
        strcat(outPath[i],"/");
        sprintf(tmp,"%d",i);
        strcat(outPath[i],tmp);
        ofp[i]=fopen(outPath[i],"w");
        //printf("%s\n",outPath[i]);
        if(!ofp[i])
        {
            printf("open outpath error");
            return -1;
        }
    }
    int value=0,count=0,hash=0;
    while(~fscanf(ifp,"%d",&value))
    {
        hash=count%FILENUM;
        fprintf(ofp[hash],"%d\n",value);
        count++;
    }
    for(int i=0;i<FILENUM;i++) fclose(ofp[i]);
    fclose(ifp);
    return 0;
}

int spliter::doSort()
{
    timeval start,end;
    gettimeofday(&start,NULL);

    for(int i=0;i<FILENUM;i++)
    {
        ifp=fopen(outPath[i],"r");
        if(!ifp)
        {
            printf("[sort] open path %s error\n",outPath[i]);
            return -1;
        }
        vec.clear();
        int value=0;
        while(~fscanf(ifp,"%d",&value))
        {
            vec.push_back(value);
        }
        fclose(ifp);
        sort(vec.begin(),vec.end());
        ofp[i]=fopen(outPath[i],"w");
        if(!ofp[i])
        {
            printf("[sort] open path %s error\n",outPath[i]);
            return -1;
        }
        for(int j=0;j<vec.size();j++) fprintf(ofp[i],"%d\n",vec[j]);
        fclose(ofp[i]);
    }

    gettimeofday(&end,NULL);

    printf("sort time : %.9fs\n",(end.tv_sec-start.tv_sec) + (end.tv_usec-start.tv_usec)/1000000.0);

    return 0;
}

void* spliter::singleSort(void* arg)
{
    Arg *ag = (Arg *)arg;

    spliter *ts=ag->ts;
    int index = ag->index;
    delete ag;

    FILE *fp = fopen(ts->outPath[index],"rt+");
    if(!fp)
    {
        printf("[thread] open path %s error\n",ts->outPath[index]);
        return NULL;
    }
    vector<int> container;
    int value=0;
    while(~fscanf(fp,"%d",&value))
    {
        container.push_back(value);
    }
    if(0!=fseek(fp,0,SEEK_SET))
    {
        printf("[thread] fseek error\n");
        return NULL;
    }
    if(0!=truncate(ts->outPath[index],0))
    {
        printf("[thread] truncate error\n");
        return NULL;
    }
    sort(container.begin(),container.end());
    for(size_t i=0;i<container.size();i++)
    {
        fprintf(fp,"%d\n",container[i]);
    }
    fclose(fp);
    return NULL;
}
int spliter::doMultipleSort()
{
    timeval start,end;

    gettimeofday(&start,NULL);
    for(int i=0,ret=0;i<FILENUM;i++)
    {
        Arg *ag = new Arg;
        if(!ag)
        {
            printf("ag is NULL\n");
            return -1;
        }
        ag->ts=this;
        ag->index=i;
        ret=pthread_create(&task[i],NULL,singleSort,ag);
        if(0!=ret)
        {
            printf("pthread %d create error with errorcode %d\n",i,ret);
            return -1;
        }
        printf("thread %d created.\n",i);
    }
    for(int i=0,ret=0;i<FILENUM;i++)
    {
        ret=pthread_join(task[i],NULL);
        if(0!=ret)
        {
            printf("pthread join error\n");
            return -1;
        }
        printf("thread %d joined.\n",i);
    }
    gettimeofday(&end,NULL);
    printf("sort time : %.9fs\n",(end.tv_sec-start.tv_sec) + (end.tv_usec-start.tv_usec)/1000000.0);
    return 0;
}

int spliter::doMerge(const char* finalPath)
{
    if(!finalPath)
    {
        printf("finalPath is NULL\n");
        return -1;
    }
    FILE *fp = fopen(finalPath,"w");
    if(!fp)
    {
        printf("open finalPath %s error\n",finalPath);
        return -1;
    }
    priority_queue<Item,vector<Item> > q;
    int value=0;
    for(int i=0;i<FILENUM;i++)
    {
        ofp[i] = fopen(outPath[i],"r");
        if(!ofp[i])
        {
            printf("[merge] open outPath %s error\n",outPath[i]);
            return -1;
        }
        if(fscanf(ofp[i],"%d",&value)!=EOF)
        {
            q.push(Item(i,value));
        }
    }
    Item u(0,0);
    int linenum=0;
    while(!q.empty())
    {
        u=q.top();
        q.pop();
        fprintf(fp,"%d\t%d\n",++linenum,u.value);
        //fprintf(fp,"%d\n",u.value);
        if(fscanf(ofp[u.index],"%d",&value)!=EOF)
        {
            q.push(Item(u.index,value));
        }
    }
    for(int i=0;i<FILENUM;i++)
    {
        fclose(ofp[i]);
    }
    fclose(fp);
    return 0;
}

